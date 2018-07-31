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
			std::vector<Block> row;
			while (sstream >> tileCode)// Reading each tileID(block type) separated by spaces
			{
				row.push_back(static_cast<Block>(tileCode));
			}
			m_world.push_back(row);
		}
	}	
	if (m_world.size() > 0)
		this->init(worldWidth, worldHeight);
}

void GameWorld::Generate(GLuint worldWidth, GLuint worldHeight)
{
	//Random world generation code goes here
}

void GameWorld::Draw(SpriteRenderer &renderer)
{
	//Looping through the whole map
	//TODO: change this so we only loop through visible tiles
	//TODO: implement chunk based rendering
	//GLfloat blockWidth = m_worldWidthInPixels / m_worldWidthInTiles, 
	//GLfloat blockHeight = m_worldHeightInPixels / m_worldHeightInTiles;
	
	//this->Position, this->Size, this->Rotation, this->Color
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
				tex = ResourceManager::GetTexture(ResourceManager::BLOCK_TEX);
				break;
			case ResourceManager::BLOCK_SOLID_TEX:
				tex = ResourceManager::GetTexture(ResourceManager::BLOCK_SOLID_TEX);
				break;
			case ResourceManager::GRASS_TEX:
				tex = ResourceManager::GetTexture(ResourceManager::GRASS_TEX);
				break;
			case ResourceManager::STONE_TEX:
				tex = ResourceManager::GetTexture(ResourceManager::STONE_TEX);
				break;
			case ResourceManager::DIRT_TEX:
				tex = ResourceManager::GetTexture(ResourceManager::DIRT_TEX);
				break;
			default: 
				break;
			}

			renderer.DrawSprite(tex, pos, size, 0.0f, glm::vec3(1.0f));
		}
	}

}

void GameWorld::init(GLuint worldWidth, GLuint worldHeight)
{
	m_worldHeightInPixels = worldHeight;
	m_worldWidthInPixels = worldWidth;

	m_worldHeightInTiles = m_world.size();
	m_worldWidthInTiles = m_world[0].size(); // Note we can index vector at [0] since this function is only called if height > 0

	//Here i want to generate VAOs for the world map
	//
}

