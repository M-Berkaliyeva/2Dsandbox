#ifndef GAME_H
#define GAME_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "GameWorld.h"

/*******************************************************************
	Game class holds all game related states and functionality,
	while separating the windowing code for easier portability and 
	maintainability.
	Hosts main game loop: render, update and input processing
********************************************************************/

class Game
{
public:
	// Represents the current state of the game
	enum GameState
	{
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN//probably wont need this for now
		//TODO: add more states like GAME_PAUSE etc
	};
public:
	// Game state
	GameState	State;
	GLboolean	Keys[1024];
	GLuint		Width, Height;
	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
private:
	SpriteRenderer * Renderer;
	GameWorld m_world;
};

#endif