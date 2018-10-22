#pragma once
#include "GameObject.h"
#include "InputManager.h"

class Player :	public GameObject
{
private:
	static const float PLAYER_SPRITE_WIDTH;
	static const float PLAYER_SPRITE_HEIGHT;
	static const float TILE_SIZE_IN_PIXELS;
public:
	enum PlayerState
	{
		PLAYER_STANDING,
		PLAYER_WALKING,
		PLAYER_JUMPING,
		PLAYER_FALLING
	};

	Player();
	~Player();

	void OnLoad(glm::vec2 pos, Texture2D sprite);
	void Update(GLfloat deltaTime);

	void SetPlayerState(PlayerState newState) { m_playerCurrentState = newState; }
	PlayerState GetPlayerState() { return m_playerCurrentState; }

private: 
	void jump();

	PlayerState m_playerCurrentState;
	GLfloat m_jumpSpeed;
	GLfloat m_maxFallingSpeed;
	GLfloat m_walkSpeed;

};

