#include "Player.h"

const float Player::TILE_SIZE_IN_PIXELS = 16.0f;
const float Player::PLAYER_SPRITE_WIDTH = TILE_SIZE_IN_PIXELS * 3;
const float Player::PLAYER_SPRITE_HEIGHT = TILE_SIZE_IN_PIXELS * 3;

Player::Player()
{	
}


Player::~Player()
{
}

void Player::OnLoad(glm::vec2 pos)
{
	m_objectType = OBJECT_TYPE_PLAYER;
	m_position = pos;
	m_size = glm::vec2(PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT);
	m_speed = glm::vec2(0.0f, 0.0f);
	m_acceleration = glm::vec2(0.0f, 0.0f);
	m_collisionRect = glm::vec4(m_size.x / 4, 0.0f, m_size.x / 2, m_size.y);
	m_tilesheet = ResourceManager::GetTileSheet(ResourceManager::PLAYER_TILESHEET);
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
	m_animationSpeed = 0.1f;
}

void Player::Update(GLfloat deltaTime)
{
	InputManager &inputManager = InputManager::instance();

	switch (m_playerCurrentState)
	{
	case PLAYER_STANDING:
		m_currAnimationFrame = 2;
		m_animationFramesCount = 4;
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
		else if (inputManager.isKeyDown(GLFW_KEY_SPACE))
		{
			m_speed.y = m_jumpSpeed;
			m_isGrounded = false;
			m_playerCurrentState = PLAYER_JUMPING;
			break;
		}
		break;
	case PLAYER_WALKING:
		m_currAnimationFrame = 10;
		m_animationFramesCount = 6;
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
			{ 
				m_speed.x = -m_walkSpeed;
				m_direction = -1;
			}
							
		}
		else if(inputManager.isKeyDown(GLFW_KEY_D))
		{
			if (m_isHittingRightWall)
				m_speed.x = 0.0f;
			else
			{
				m_speed.x = m_walkSpeed;
				m_direction = 1;
			}
				
		}

		if (inputManager.isKeyDown(GLFW_KEY_SPACE))
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
		if (m_speed.y < 0)//if jumping
		{
			m_currAnimationFrame = 6;
			m_animationFramesCount = 4;
		}
		else // if falling
		{
			m_currAnimationFrame = 0;
			m_animationFramesCount = 2;
		}		
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
			{
				m_speed.x = -m_walkSpeed;
				m_direction = -1;
			}
				
			
		}
		else if (inputManager.isKeyDown(GLFW_KEY_D))
		{
			if (m_isHittingRightWall)
				m_speed.x = 0.0f;
			else
			{
				m_speed.x = m_walkSpeed;
				m_direction = 1;
			}				
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
