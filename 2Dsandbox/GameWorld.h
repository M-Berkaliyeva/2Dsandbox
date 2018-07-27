#ifndef GAME_WORLDL_H
#define GAME_WORLD_H

#include <vector>

#include <GL/glew.h>

#include "SpriteRenderer.h"

class GameWorld
{
public:
	const enum Block
	{
		EMPTY_BLOCK = 0,
		GRASS_BLOCK,
		DIRT_BLOCK,
		STONE_BLOCK
	};

	static const int WorldWidthInTiles = 10;
	static const int WorldHeightInTiles = 10;

	// Loads world data from file
	void Load(const char* file, GLuint worldWidth, GLuint worldHeight);
	// Render world
	void Draw(SpriteRenderer &renderer);

private:
	std::vector<Block> m_world;
};
#endif