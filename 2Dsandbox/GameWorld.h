#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <vector>

#include <GL/glew.h>

#include "SpriteRenderer.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "SpriteBatchRenderer.h"
#include "Camera2D.h"

class GameWorld
{
public:
	const GLfloat m_tileWidth = 16.0f;
	const GLfloat m_tileHeight = 16.0f;

	GLuint m_worldWidthInTiles = 0;
	GLuint m_worldHeightInTiles = 0;

	GLfloat m_worldWidthInPixels = 0;
	GLfloat m_worldHeightInPixels = 0;

	const enum Block
	{
		EMPTY_BLOCK = 0,
		GRASS_BLOCK,
		DIRT_BLOCK,
		STONE_BLOCK
	};

	GameWorld() {};

	// Loads world data from file
	void Load(const char* file, GLuint worldWidth, GLuint worldHeight);
	// Randomly generates world map given dimensions
	void Generate(GLuint worldWidth, GLuint worldHeight);
	// Render world
	void Draw(SpriteRenderer &renderer);
	// Render world in batches
	void DrawInBatches(const SpriteRenderer &renderer, Camera2D &cam);

	const ResourceManager::TileName getTileAtPos(GLfloat xPos, GLfloat yPos);
	void setTileAtPos(GLfloat xPos, GLfloat yPos, ResourceManager::TileName tileName);

private:
	// Initialize level from tile data
	void Init(GLuint levelWidth, GLuint levelHeight);
	
	//std::vector<std::vector<Block>> m_world;
	std::vector<std::vector<ResourceManager::TileName>> m_world;
	SpriteBatchRenderer m_spriteBatch;

	// Offsets to do coordinate fliping stuff
	GLuint m_offsetTilesHeight;
};
#endif