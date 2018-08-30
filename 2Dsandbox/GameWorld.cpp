#include "GameWorld.h"

#include <fstream>
#include <sstream>

void GameWorld::Load(const char * file, GLuint worldWidth, GLuint worldHeight)
{
	// Clear old map
	this->m_world.clear();
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
			m_world.push_back(row);
		}
	}	
	if (m_world.size() > 0)
		this->Init(worldWidth, worldHeight);
}

void GameWorld::Generate(GLuint worldWidth, GLuint worldHeight)
{
	//Random world generation code goes here
	// I want to generate map one column at a time, to make hills 
	//int groundHeight = worldHeight / 2;//start in the middle
	m_world.resize(worldWidth);
	for (int x = 0; x < worldWidth; x++)
	{
		m_world[x].resize(worldHeight);
		// Go one tile up or one tile down
		//groundHeight += rand() % 1 - 1;
		//for (int y = worldHeight - 1; y >= 0; y--)
		for(int y = 0; y < worldHeight; y++)
		{	
			// Fill ground tiles up to set height -> rest is empty
			if(y == 5)
				m_world[x][y] = ResourceManager::GRASS_TILE;
			else
				m_world[x][y] = ResourceManager::STONE_TILE;			
		}
	}
	if (m_world.size() > 0)
		this->Init(worldWidth, worldHeight);
}

void GameWorld::Draw(SpriteRenderer &renderer)
{
	//Looping through the whole map
	//TODO: change this so we only loop through visible tiles
	//TODO: implement chunk based rendering
	//GLfloat blockWidth = m_worldWidthInPixels / m_worldWidthInTiles, 
	//GLfloat blockHeight = m_worldHeightInPixels / m_worldHeightInTiles;
	
	//this->Position, this->Size, this->Rotation, this->Color
	/*
	for (int y = 0; y < m_worldHeightInTiles; y++)
	{
		for (int x = 0; x < m_worldWidthInTiles; x++)
		{
			glm::vec2 pos = glm::vec2(x*m_tileWidth, y*m_tileHeight);
			glm::vec2 size = glm::vec2(m_tileWidth, m_tileHeight);//This can be done only once
			Texture2D tex;

			switch (m_world[y][x])
			{
			case ResourceManager::BLOCK_TEX:
				tex = ResourceManager::GetSpritesheet(ResourceManager::BLOCK_TEX);
				break;
			case ResourceManager::BLOCK_SOLID_TEX:
				tex = ResourceManager::GetSpritesheet(ResourceManager::BLOCK_SOLID_TEX);
				break;
			case ResourceManager::GRASS_TEX:
				tex = ResourceManager::GetSpritesheet(ResourceManager::GRASS_TEX);
				break;
			case ResourceManager::STONE_TEX:
				tex = ResourceManager::GetSpritesheet(ResourceManager::STONE_TEX);
				break;
			case ResourceManager::DIRT_TEX:
				tex = ResourceManager::GetSpritesheet(ResourceManager::DIRT_TEX);
				break;
			default: 
				break;
			}

			renderer.DrawSprite(tex, pos, size, 0.0f, glm::vec3(1.0f));
		}
	}
	*/

}

void GameWorld::DrawInBatches(const SpriteRenderer &renderer, Camera2D &cam)
{
	//TODO: implement chunk based rendering -? do i need to tho?

	// Code to precalculate visible bounds of world
	int startCol = ceil(cam.getPosition().x / m_tileWidth);
	int endCol = startCol + (cam.getDimensions().x / m_tileWidth);
	int startRow = ceil(cam.getPosition().y / m_tileHeight);
	int endRow = startRow + (cam.getDimensions().y / m_tileHeight);

	//Get camera offsets
	float offsetX = -cam.getPosition().x + startCol * m_tileWidth;
	float offsetY = -cam.getPosition().y + startRow * m_tileHeight;

	//Change fliiping offset
	m_offsetTilesHeight = endRow;
	
	//Start batching sprites
	m_spriteBatch.begin();

	// Loop through visible part of map and add tiles to batch
	for (int y = startRow; y < endRow; y++)//for (int y = 0; y < m_worldHeightInTiles; y++)//
	{
		for (int x = startCol; x < endCol; x++)//for (int x = 0; x < m_worldWidthInTiles; x++)//
		{	
			// Get spritesheet for our tiles
			// Get uv lookup table accordingly
			static Texture2D tex = ResourceManager::GetSpritesheet(ResourceManager::TILES_SPRITESHEET);
			ResourceManager::TexParams uvParams = ResourceManager::GetTileParams(ResourceManager::TILES_SPRITESHEET, m_world[x][y]);
			
			//uv = glm::vec4(uvParams.u, uvParams.v, uvParams.width, uvParams.height);
			
			float posX = (x - startCol) * m_tileWidth + offsetX;
			// This one is tricky, since we need to go around the idea that camera[0,0] is bottom left
			// and in our tile map vector [0,0] is top left tile
			// we need to flip y position from top to bottom
			float posY = ((m_offsetTilesHeight - y - startRow) * m_tileHeight + offsetY) - m_tileHeight;
			
			//var x = (c - startCol) * map.tsize + offsetX;
			//var y = (r - startRow) * map.tsize + offsetY;

			glm::vec4 pos(posX, posY, m_tileWidth, m_tileHeight);
			glm::vec4 uv(uvParams.u, uvParams.v, uvParams.width, uvParams.height);
			Color col; col.r = 255; col.g = 255; col.b = 255; col.a = 255;

			m_spriteBatch.draw(pos, uv, tex.ID, 0.0f, col);
		}
	}
	// Once we added all tiles to batch start rendering
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}

const ResourceManager::TileName GameWorld::getTileAtPos(GLfloat xPos, GLfloat yPos)
{
	// Assuming already accounted for camera position
	GLuint xIndex = static_cast<GLuint>(xPos / m_tileWidth);
	GLuint yIndex = m_offsetTilesHeight - static_cast<GLuint>((yPos + m_tileHeight) / m_tileHeight); // Hacking around bottom-left coord system - sorry
	
	// Make sure we click on map
	if (xIndex < m_worldWidthInTiles && yIndex < m_worldHeightInTiles)
		return m_world[xIndex][yIndex];
}

void GameWorld::setTileAtPos(GLfloat xPos, GLfloat yPos, ResourceManager::TileName tileName)
{
	// Assuming already accounted for camera position
	GLuint xIndex = static_cast<GLuint>(xPos / m_tileWidth);
	GLuint yIndex = m_offsetTilesHeight - static_cast<GLuint>((yPos + m_tileHeight)/ m_tileHeight); // Hacking around bottom-left coord system - sorry

	// Make sure we click on map
	if(xIndex < m_worldWidthInTiles && yIndex < m_worldHeightInTiles)
		m_world[xIndex][ yIndex] = tileName;
}

void GameWorld::Init(GLuint worldWidth, GLuint worldHeight)
{
	m_worldHeightInPixels = worldHeight * m_tileHeight;
	m_worldWidthInPixels = worldWidth * m_tileWidth;

	m_worldHeightInTiles = worldHeight; 
	m_worldWidthInTiles = worldWidth;

	m_offsetTilesHeight = m_worldHeightInTiles;
	
	//Here i want to generate VAOs for the world map
	m_spriteBatch.init();
}

