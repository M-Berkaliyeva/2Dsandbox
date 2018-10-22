#include "Player.h"

const float Player::TILE_SIZE_IN_PIXELS = 16.0f;
const float Player::PLAYER_SPRITE_WIDTH = TILE_SIZE_IN_PIXELS * 2;
const float Player::PLAYER_SPRITE_HEIGHT = TILE_SIZE_IN_PIXELS * 3;

Player::Player()
{	
}


Player::~Player()
{
}

void Player::OnLoad(glm::vec2 pos, Texture2D sprite)
{
	m_objectType = OBJECT_TYPE_PLAYER;
	m_position = pos;
	m_size = glm::vec2(PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT);
	m_speed = glm::vec2(0.0f, 0.0f);
	m_acceleration = glm::vec2(0.0f, 0.0f);
	m_collisionRect = glm::vec4(0.0f, 0.0f, m_size.x, m_size.y);
	m_sprite = sprite;
	m_isCollidable = true;
	m_isGravityApplied = true;
	m_isGrounded = false;
	m_wasGrounded = false;
	m_isHittingCeiling = false;
	m_wasHittingCeiling = false;
	m_isHittingLeftWall = false;
	m_wasHittingLeftWall = false;
	m_isHittingRightWall = false;
	m_wasHittingRightWall = false;
	m_playerCurrentState = PLAYER_STANDING;
	m_walkSpeed = 200.0f;
	m_jumpSpeed = -400.0f;
	m_maxFallingSpeed = 400.0f;
}

void Player::Update(GLfloat deltaTime)
{
	InputManager &inputManager = InputManager::instance();

	switch (m_playerCurrentState)
	{
	case PLAYER_STANDING:
		m_speed = glm::vec2(0.0f, 0.0f);
		m_acceleration.y = 0.0f;
		//handle standing animation here
		if (!m_isGrounded)
		{
			m_playerCurrentState = PLAYER_JUMPING;
			break;
		}
		if (inputManager.isKeyDown(GLFW_KEY_A) != inputManager.isKeyDown(GLFW_KEY_D))
		{			
			m_playerCurrentState = PLAYER_WALKING;
			break;
		}		
		else if (inputManager.isKeyDown(GLFW_KEY_W))
		{
			m_speed.y = m_jumpSpeed;
			m_isGrounded = false;
			m_playerCurrentState = PLAYER_JUMPING;
			break;
		}
		break;
	case PLAYER_WALKING:
		
		// If neither or both lef and right are pressed -> stop walking
		if (inputManager.isKeyDown(GLFW_KEY_A) == inputManager.isKeyDown(GLFW_KEY_D))
		{
			m_speed = glm::vec2(0.0f, 0.0f);
			m_playerCurrentState = PLAYER_STANDING;
			break;
		}
		else if(inputManager.isKeyDown(GLFW_KEY_A))
		{
			if (m_isHittingLeftWall)
				m_speed.x = 0.0f;
			else
				m_speed.x = -m_walkSpeed;			
		}
		else if(inputManager.isKeyDown(GLFW_KEY_D))
		{
			if (m_isHittingRightWall)
				m_speed.x = 0.0f;
			else			
				m_speed.x = m_walkSpeed;
		}

		if (inputManager.isKeyDown(GLFW_KEY_W))
		{
			m_speed.y = m_jumpSpeed;
			m_isGrounded = false;
			m_playerCurrentState = PLAYER_JUMPING;
			break;
		}
		else if (!m_isGrounded)
		{
			m_playerCurrentState = PLAYER_JUMPING;
			break;
		}
		break;	
	case PLAYER_JUMPING:
		jump();
		if (inputManager.isKeyDown(GLFW_KEY_A) == inputManager.isKeyDown(GLFW_KEY_D))
		{
			m_speed.x = 0.0f;			
		}
		else if (inputManager.isKeyDown(GLFW_KEY_A))
		{
			if (m_isHittingLeftWall)
				m_speed.x = 0.0f;
			else			
				m_speed.x = -m_walkSpeed;
			
		}
		else if (inputManager.isKeyDown(GLFW_KEY_D))
		{
			if (m_isHittingRightWall)
				m_speed.x = 0.0f;
			else
				m_speed.x = m_walkSpeed;
		}
		if (m_isGrounded)
		{
			m_playerCurrentState = PLAYER_STANDING;
		}
		break;
	}	

	GameObject::Update(deltaTime);
}

void Player::jump()
{
	m_acceleration.y = GRAVITY;
	if (m_speed.y > m_maxFallingSpeed)
		m_speed.y = m_maxFallingSpeed;
}
