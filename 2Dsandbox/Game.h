#ifndef GAME_H
#define GAME_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameWorld.h"
#include "Player.h"
#include "Camera2D.h"

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
	GLfloat GameTime;
	GameState	State;
	GLuint		Width, Height;
	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void Update(GLfloat dt);
	void Render(GLFWwindow* window);

private:
	void InitShaders();
	void InitTextures();

	SpriteRenderer * Renderer;
	SpriteBatchRenderer * m_batchRenderer;
	//GameWorld m_world;
	//Camera2D m_camera;
	Player m_player;

	// MOve bg files to separate class -> willl be quite comlpex
	Texture2D	m_bgSprite;
	GLfloat m_bgUVu;
	GLfloat m_bgspeed;
};

#endif