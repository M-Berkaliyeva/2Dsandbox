#include "GameWorld.h"

#include <fstream>
#include <sstream>
#include <random>

//-------------------------------------------------------------------------- //
GameWorld* GameWorld::s_pInstance = NULL;
//-------------------------------------------------------------------------- //

GLboolean GameWorld::initialize()
{
	if (s_pInstance)
		return GL_FALSE;

	s_pInstance = new GameWorld();

	if (!s_pInstance)
		return GL_FALSE;

	//s_pInstance->initializeInternal();
	return GL_TRUE;
}

GameWorld::GameWorld()
{
}


GameWorld::~GameWorld()
{
}

void GameWorld::Load(const char * file, GLuint worldWidth, GLuint worldHeight)
{
	// Clear old map
	this->m_worldMap.clear();
	// Load from file
	GLuint tileCode;
	std::string line;
	std::ifstream fstream(file);
	if (fstream)
	{
		while (std::getline(fstream, line))// reading each line of level
		{
			std::istringstream sstream(line);
			//std::vector<Block> row;
			std::vector<ResourceManager::TileType> row;
			while (sstream >> tileCode)// Reading each tileID(block type) separated by spaces
			{
				//row.push_back(static_cast<Block>(tileCode));
				row.push_back(static_cast<ResourceManager::TileType>(tileCode));
			}
			m_worldMap.push_back(row);
		}
	}	
	if (m_worldMap.size() > 0)
		this->Init(m_worldMap.size(), m_worldMap[0].size());
}

void GameWorld::CreateMap(GLuint worldWidth, GLuint worldHeight)
{
	Generate(worldWidth, worldHeight);
	GenerateCavesCellularAutomata(4);
	GenerateHillsRandomWalk(50);
}

void GameWorld::Generate(GLuint worldWidth, GLuint worldHeight)
{
	m_worldMap.resize(worldWidth);
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> tileDistr(ResourceManager::DIRT_TILE, ResourceManager::STONE_TILE);
	std::uniform_real_distribution<> deadOrAliveDistr(0.0, 1.0);
	float chanceToStartAlive = 0.4f;
	for (int x = 0; x < worldWidth; x++)
	{
		m_worldMap[x].resize(worldHeight);

		for(int y = 0; y < worldHeight; y++)
		{
			if (x == 0 || x == worldWidth - 1 || y == 0 || y == worldHeight - 1)
				m_worldMap[x][y] = ResourceManager::DIRT_TILE;//place tiles around the border
			else if (deadOrAliveDistr(gen) < chanceToStartAlive)
				m_worldMap[x][y] = (ResourceManager::TileType)tileDistr(gen);//random non empty tile
			else
				m_worldMap[x][y] = ResourceManager::EMPTY_TILE;				
		}
	}
	if (m_worldMap.size() > 0)
		this->Init(worldWidth, worldHeight);
}

void GameWorld::GenerateHillsRandomWalk(int startHeight)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> heightDistr(-1, 1);
	GLuint topHeight = startHeight;
	GLuint dirtOffset = 10;
	for (int x = 0; x < m_worldWidthInTiles; x++)
	{
		topHeight += heightDistr(gen);

		for (int y = 0; y < m_worldHeightInTiles; y++)
		{
			if (y == topHeight)
				m_worldMap[x][y] = ResourceManager::GRASS_TILE;
			else if (y < topHeight)
				m_worldMap[x][y] = ResourceManager::EMPTY_TILE;
			else if(y > topHeight && y < topHeight + dirtOffset)
				m_worldMap[x][y] = ResourceManager::DIRT_TILE;
		}
	}
}

void GameWorld::GenerateCavesCellularAutomata(GLuint iterations)
{
	std::vector<std::vector<ResourceManager::TileType>> newWorldMap;
	newWorldMap.resize(m_worldWidthInTiles);
	int birthLimit = 4;
	int deathLimit = 3;
	for (int i = 0; i < iterations; i++)
	{
		// Loop through the map
		for (int x = 0; x < m_worldWidthInTiles; x++)
		{
			newWorldMap[x].resize(m_worldHeightInTiles);
			for (int y = 0; y < m_worldHeightInTiles; y++)
			{
				int neighbourCount = GetNeighboursCount(x, y);
				// First, if a cell is alive but has too few neighbours, kill it.
				if(m_worldMap[x][y] != ResourceManager::EMPTY_TILE)
				{
					if (x == 0 || x == m_worldWidthInTiles - 1 || y == 0 || y == m_worldHeightInTiles - 1)
						newWorldMap[x][y] = m_worldMap[x][y];//place tiles around the border
					else if (neighbourCount < deathLimit)
						newWorldMap[x][y] = ResourceManager::EMPTY_TILE;
					else
						newWorldMap[x][y] = m_worldMap[x][y];
				}
				// Otherwise, if the cell is dead now, check if it has the right number of neighbours to be 'born'
				else 
				{
					if (neighbourCount > deathLimit)
						newWorldMap[x][y] = ResourceManager::DIRT_TILE;
					else
						newWorldMap[x][y] = ResourceManager::EMPTY_TILE;
				}				
			}
		}
		m_worldMap = newWorldMap;//apply changes to world map for this iteration
	}
}

GLuint GameWorld::GetNeighboursCount(GLuint tileX, GLuint tileY)
{
	GLuint neighboursCount = 0;
	
	for (int neightbourX = tileX - 1; neightbourX <= tileX + 1; neightbourX++)
	{
		for (int neightbourY = tileY - 1; neightbourY <= tileY + 1; neightbourY++)
		{
			if (neightbourX == tileX && neightbourY == tileY)
			{
				// Do nothing if we are looking at middle tile
			}
			else if (neightbourX >= 0 && neightbourX < m_worldWidthInTiles && neightbourY >= 0 && neightbourY < m_worldHeightInTiles)
			{
				if (m_worldMap[neightbourX][neightbourY] != ResourceManager::EMPTY_TILE)
				{
					neighboursCount++;
				}
			}
			else 
			{
				neighboursCount++;
			}			
		}
	}	
	return neighboursCount;
}

void GameWorld::DrawInBatches(SpriteBatchRenderer &renderer)
{
	//TODO: implement chunk based rendering -? do i need to tho?
	Camera2D &cam = Camera2D::instance();
	// Code to precalculate visible tile bounds of world
	int startCol = floor(cam.getPosition().x / m_tileWidth);
	int endCol = startCol + (cam.getDimensions().x / m_tileWidth);
	int startRow = floor(cam.getPosition().y / m_tileHeight);
	int endRow = startRow + (cam.getDimensions().y / m_tileHeight);

	//Get camera offsets
	float offsetX = -cam.getPosition().x + startCol * m_tileWidth;
	float offsetY = -cam.getPosition().y + startRow * m_tileHeight;


	// Loop through visible part of map and add tiles to batch
	for (int y = startRow; y <= endRow; y++)
	{
		for (int x = startCol; x <= endCol; x++)
		{
			// Get spritesheet for our tiles
			// Get uv lookup table accordingly
			static TileSheet tilesheet = ResourceManager::GetTileSheet(ResourceManager::TILES_TILESHEET);
			ResourceManager::TexCoords uvParams = ResourceManager::GetTileTexCoords(ResourceManager::TILES_TILESHEET, m_worldMap[x][y]);

			float posX = (x - startCol) * m_tileWidth +offsetX;
			float posY = (y - startRow) * m_tileHeight +offsetY;

			glm::vec4 pos(posX, posY, m_tileWidth, m_tileHeight);
			glm::vec4 uv(uvParams.u, uvParams.v, uvParams.width, uvParams.height);
			Color col; col.r = 255; col.g = 255; col.b = 255; col.a = 255;

			renderer.draw(pos, uv, tilesheet.texture.ID, 0.0f, col);
		}
	}
	// Once we added all tiles to batch start rendering
	glm::mat4 model = glm::mat4(1.0);
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).SetMatrix4("model", model);
}

const glm::vec2 GameWorld::getPositionOfTile(GLuint indexX, GLuint indexY)
{	
	Camera2D &cam = Camera2D::instance();
	return glm::vec2(
		(float)(indexX * m_tileWidth) + cam.getPosition().x,	
		(float)(indexY * m_tileHeight) + cam.getPosition().y
	);
}

const ResourceManager::TileType GameWorld::getTileTypeAtPos(GLfloat xPos, GLfloat yPos)
{
	Camera2D &cam = Camera2D::instance();
	// Accounting for camera position
	GLuint xIndex = static_cast<GLuint>((xPos + cam.getPosition().x) / m_tileWidth);
	GLuint yIndex = static_cast<GLuint>((yPos + cam.getPosition().y) / m_tileHeight);
	
	// Make sure we click on map
	if (xIndex < m_worldWidthInTiles && yIndex < m_worldHeightInTiles)
		return m_worldMap[xIndex][yIndex];
}

const ResourceManager::TileType GameWorld::getTileType(GLuint indexX, GLuint indexY)
{	
	//If indexs out of map bounds -> return as obstacle
	if (indexX < 0 || indexX >= m_worldWidthInTiles
		|| indexY < 0 || indexY >= m_worldHeightInTiles)
		return ResourceManager::DIRT_TILE;// any non empty tile will work
	return m_worldMap[indexX][indexY];
}

const GLboolean GameWorld::isSolidTile(GLuint indexX, GLuint indexY)
{
	//If indexs out of map bounds -> return as obstacle
	if (indexX < 0 || indexX >= m_worldWidthInTiles
		|| indexY < 0 || indexY >= m_worldHeightInTiles)
		return true;
	//for now all tiles apart from empty tile are obstacles-> maybe water and others will not be?
	return (m_worldMap[indexX][indexY] != ResourceManager::EMPTY_TILE); 
}

const GLboolean GameWorld::isEmptyTile(GLuint indexX, GLuint indexY)
{
	//If indexs out of map bounds -> return as obstacle
	if (indexX < 0 || indexX >= m_worldWidthInTiles
		|| indexY < 0 || indexY >= m_worldHeightInTiles)
		return false;
	//for now all tiles apart from empty tile are obstacles-> maybe water and others will not be?
	return (m_worldMap[indexX][indexY] == ResourceManager::EMPTY_TILE);
}

const glm::ivec2 GameWorld::getTileIndexAtPos(GLfloat xPos, GLfloat yPos)
{
	Camera2D &cam = Camera2D::instance();
	return glm::ivec2(
		static_cast<GLuint>((xPos)/ m_tileWidth),
		static_cast<GLuint>((yPos)/ m_tileHeight)
	);
}

void GameWorld::setTileAtPos(GLfloat xPos, GLfloat yPos, ResourceManager::TileType tileName)
{
	Camera2D &cam = Camera2D::instance();
	// Accounting for camera position
	GLuint xIndex = static_cast<GLuint>((xPos + cam.getPosition().x) / m_tileWidth);
	GLuint yIndex = static_cast<GLuint>((yPos + cam.getPosition().y)/ m_tileHeight);

	// Make sure we click on map
	if(xIndex < m_worldWidthInTiles && yIndex < m_worldHeightInTiles)
		m_worldMap[xIndex][ yIndex] = tileName;
}

void GameWorld::Init(GLuint worldWidth, GLuint worldHeight)
{
	m_worldHeightInPixels = worldHeight * m_tileHeight;
	m_worldWidthInPixels = worldWidth * m_tileWidth;

	m_worldHeightInTiles = worldHeight; 
	m_worldWidthInTiles = worldWidth;

	m_offsetTilesHeight = m_worldHeightInTiles;	
}

