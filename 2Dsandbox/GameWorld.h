#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <vector>

#include <GL/glew.h>

//#include "SpriteRenderer.h"
//#include "GameObject.h"
#include "ResourceManager.h"
#include "SpriteBatchRenderer.h"
#include "Camera2D.h"

/****************************************************************
**********************SINGLETON CLASS****************************
****************************************************************/

class GameWorld
{
public:
	
	const GLfloat m_tileWidth = 16.0f;
	const GLfloat m_tileHeight = 16.0f;

	GLuint m_worldWidthInTiles = 0;
	GLuint m_worldHeightInTiles = 0;

	GLfloat m_worldWidthInPixels = 0;
	GLfloat m_worldHeightInPixels = 0;
	
	static GameWorld* s_pInstance;

	static GLboolean initialize();
	static GameWorld& instance();

	// Loads world data from file
	void Load(const char* file, GLuint worldWidth, GLuint worldHeight);
	// Creates map
	void CreateMap(GLuint worldWidth, GLuint worldHeight);
	// Render world in batches
	//void DrawInBatches(const SpriteRenderer &renderer, Camera2D &cam);
	void DrawInBatches(SpriteBatchRenderer &renderer);

	const glm::vec2 getPositionOfTile(GLuint indexX, GLuint indexY);
	const glm::ivec2 getTileIndexAtPos(GLfloat xPos, GLfloat yPos);
	const ResourceManager::TileType getTileTypeAtPos(GLfloat xPos, GLfloat yPos);
	const ResourceManager::TileType getTileType(GLuint indexX, GLuint indexY);
	const GLboolean isSolidTile(GLuint indexX, GLuint indexY);
	const GLboolean isEmptyTile(GLuint indexX, GLuint indexY);
	
	void setTileAtPos(GLfloat xPos, GLfloat yPos, ResourceManager::TileType tileName);

	//GLboolean CheckObjectCollision(GameObject &obj);

private:
	GameWorld();
	~GameWorld();
	// Initialize level from tile data
	void Init(GLuint levelWidth, GLuint levelHeight);
	// Randomly generates world map given dimensions
	void Generate(GLuint worldWidth, GLuint worldHeight);
	// Geberate Hills using rndom walk approach
	void GenerateHillsRandomWalk(int startHeight);
	// Generates caves based on cellular automata method
	void GenerateCavesCellularAutomata(GLuint iterations);
	// Counts number of surrounding neighbour tiles
	GLuint GetNeighboursCount(GLuint tileX, GLuint tileY);
	
	// Need to have more than one layer!
	std::vector<std::vector<ResourceManager::TileType>> m_worldMap;
	//SpriteBatchRenderer m_spriteBatch;

	// Offsets to do coordinate fliping stuff
	GLuint m_offsetTilesHeight;
};

// Inline methods
//-------------------------------------------------------------------------- //
inline GameWorld& GameWorld::instance()
{
	return *s_pInstance;
}
#endif