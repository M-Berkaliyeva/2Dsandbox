#include "GameWorld.h"

#include <fstream>
#include <sstream>
#include <random>

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
			std::vector<ResourceManager::TileName> row;
			while (sstream >> tileCode)// Reading each tileID(block type) separated by spaces
			{
				//row.push_back(static_cast<Block>(tileCode));
				row.push_back(static_cast<ResourceManager::TileName>(tileCode));
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
				m_worldMap[x][y] = (ResourceManager::TileName)tileDistr(gen);//random non empty tile
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
	std::vector<std::vector<ResourceManager::TileName>> newWorldMap;
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

/*void GameWorld::DrawInBatches(const SpriteRenderer &renderer, Camera2D &cam)
{
	//TODO: implement chunk based rendering -? do i need to tho?

	// Code to precalculate visible tile bounds of world
	int startCol = floor(cam.getPosition().x / m_tileWidth);
	int endCol = startCol + (cam.getDimensions().x / m_tileWidth);
	int startRow = floor(cam.getPosition().y / m_tileHeight);
	int endRow = startRow + (cam.getDimensions().y / m_tileHeight);
	
	//Get camera offsets
	float offsetX = -cam.getPosition().x + startCol * m_tileWidth;
	float offsetY = -cam.getPosition().y + startRow * m_tileHeight;
		
	//Start batching sprites
	m_spriteBatch.begin();

	// Loop through visible part of map and add tiles to batch
	for (int y = startRow; y <= endRow;  y++)
	{
		for (int x = startCol; x <= endCol; x++)
		{	
			// Get spritesheet for our tiles
			// Get uv lookup table accordingly
			static Texture2D tex = ResourceManager::GetSpritesheet(ResourceManager::TILES_SPRITESHEET);
			ResourceManager::TexParams uvParams = ResourceManager::GetTexParams(ResourceManager::TILES_SPRITESHEET, m_worldMap[x][y]);
						
			float posX = (x - startCol) * m_tileWidth + offsetX;
			float posY = (y - startRow) * m_tileHeight + offsetY;

			glm::vec4 pos(posX, posY, m_tileWidth, m_tileHeight);
			glm::vec4 uv(uvParams.u, uvParams.v, uvParams.width, uvParams.height);
			Color col; col.r = 255; col.g = 255; col.b = 255; col.a = 255;

			m_spriteBatch.draw(pos, uv, tex.ID, 0.0f, col);
		}
	}
	// Once we added all tiles to batch start rendering
	glm::mat4 model = glm::mat4(1.0);
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).SetMatrix4("model", model);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}*/
void GameWorld::DrawInBatches(SpriteBatchRenderer &renderer, Camera2D &cam)
{
	//TODO: implement chunk based rendering -? do i need to tho?

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
			static Texture2D tex = ResourceManager::GetSpritesheet(ResourceManager::TILES_SPRITESHEET);
			ResourceManager::TexParams uvParams = ResourceManager::GetTexParams(ResourceManager::TILES_SPRITESHEET, m_worldMap[x][y]);

			float posX = (x - startCol) * m_tileWidth + offsetX;
			float posY = (y - startRow) * m_tileHeight + offsetY;

			glm::vec4 pos(posX, posY, m_tileWidth, m_tileHeight);
			glm::vec4 uv(uvParams.u, uvParams.v, uvParams.width, uvParams.height);
			Color col; col.r = 255; col.g = 255; col.b = 255; col.a = 255;

			renderer.draw(pos, uv, tex.ID, 0.0f, col);
		}
	}
	// Once we added all tiles to batch start rendering
	glm::mat4 model = glm::mat4(1.0);
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).SetMatrix4("model", model);
}

const ResourceManager::TileName GameWorld::getTileAtPos(GLfloat xPos, GLfloat yPos)
{
	// Assuming already accounted for camera position
	GLuint xIndex = static_cast<GLuint>(xPos / m_tileWidth);
	GLuint yIndex = static_cast<GLuint>(yPos / m_tileHeight);
	
	// Make sure we click on map
	if (xIndex < m_worldWidthInTiles && yIndex < m_worldHeightInTiles)
		return m_worldMap[xIndex][yIndex];
}

void GameWorld::setTileAtPos(GLfloat xPos, GLfloat yPos, ResourceManager::TileName tileName)
{
	// Assuming already accounted for camera position
	GLuint xIndex = static_cast<GLuint>(xPos / m_tileWidth);
	GLuint yIndex = static_cast<GLuint>((yPos)/ m_tileHeight);

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
	
	//Here i want to generate VAOs for the world map
	//m_spriteBatch.init();
}

