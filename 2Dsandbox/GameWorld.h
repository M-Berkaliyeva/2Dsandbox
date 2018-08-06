#ifndef GAME_WORLDL_H
#define GAME_WORLD_H

#include <vector>

#include <GL/glew.h>

#include "SpriteRenderer.h"
#include "GameObject.h"
#include "ResourceManager.h"

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

private:
	// Initialize level from tile data
	void Init(GLuint levelWidth, GLuint levelHeight);
	
	std::vector<std::vector<Block>> m_world;	
};
#endif