#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "SpriteBatchRenderer.h"
#include "GameWorld.h"
#include "Camera2D.h"


/*****************************************************************
	Container object for holding all state relevant for a single
	game object entity. Each object in the game likely needs the
	minimal of state as described within GameObject.
	Will be used for:
	*Player
	*NPC
	*Every other non-tile object
	I don't want to use this for tile since every game object 
	will have attached sprite and therefore separate render call
	->for world tiles will use smth called chunck-based rendering
***************************************************************/
class GameObject
{
public:
	static const float GRAVITY;
	enum ObjectType
	{
		OBJECT_TYPE_GENERIC = 0,
		OBJECT_TYPE_PLAYER,
		OBJECT_TYPE_TILE_PICKUP,

		OBJECT_TYPE_COUNT
	};
	GameObject();

	virtual void Update( GLfloat deltaTime );
	virtual void Draw( SpriteBatchRenderer &renderer );
	virtual void OnCollision( GameObject* otherObject );
	virtual void OnLoad();

	void Move( GLfloat deltaTime );
	bool Collides( glm::vec4 otherCollisionRect );
	//Check collision with ground
	GLboolean HasGround(glm::vec2 oldPosition, glm::vec2 newPosition, glm::vec2 speed, float &groundY);
	GLboolean HasCeiling(glm::vec2 oldPosition, glm::vec2 newPosition, float &ceilingY);
	GLboolean HasWallOnLeft(glm::vec2 oldPosition, glm::vec2 newPosition, float &wallX);
	GLboolean HasWallOnRight(glm::vec2 oldPosition, glm::vec2 newPosition, float &wallX);

	glm::vec2 GetPosition() { return m_position; }
	glm::vec2 GetSpeed() { return m_speed; }
	GLboolean IsGrounded() { return m_isGrounded; }

protected:
	void CheckCollisionWithMap();
	void RoundVector(glm::vec2 &vecToRound); 

	ObjectType	m_objectType;
	
	// Object position and size
	glm::vec2   m_position, m_size;
	glm::vec2	m_speed, m_acceleration;
	glm::vec2	m_prevPosition, m_prevSpeed;
	// Object collision rectangle (AABB)
	glm::vec4	m_collisionRect;

	// Collision with tiles around
	GLboolean	m_isGrounded;
	GLboolean	m_wasGrounded;
	GLboolean	m_isHittingCeiling;
	GLboolean	m_wasHittingCeiling;
	GLboolean	m_isHittingLeftWall;
	GLboolean	m_wasHittingLeftWall;
	GLboolean	m_isHittingRightWall;
	GLboolean	m_wasHittingRightWall;

	GLboolean	m_isGravityApplied;
	GLboolean	m_isDrawable;
	GLboolean   m_isCollidable;
	// Render state
	Texture2D	m_sprite;// maybe should be spritesheet?
};

#endif