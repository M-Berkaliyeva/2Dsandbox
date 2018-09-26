#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
//#include "SpriteRenderer.h"
#include "SpriteBatchRenderer.h"


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
	static const float GRAVITY;
public:
	enum ObjectType
	{
		OBJECT_TYPE_GENERIC = 0,
		OBJECT_TYPE_PLAYER,
		OBJECT_TYPE_TILE_PICKUP,

		OBJECT_TYPE_COUNT
	};
	GameObject();
	GameObject(ObjectType objType,  glm::vec2 pos, glm::vec2 size, Texture2D sprite, GLboolean isCollidable = GL_TRUE, GLboolean isGravityApplied = GL_TRUE);

	virtual void Update();
	virtual void Draw(SpriteBatchRenderer &renderer);
	virtual void OnCollision(GameObject* otherObject);

	void Move(glm::vec2 speed);
	bool Collides(glm::vec4 otherCollisionRect);

private:
	ObjectType	m_objectType;
	
	// Object position and size
	glm::vec2   m_position, m_size;
	glm::vec2	m_speed, m_acceleration;

	// Object collision rectangle
	glm::vec4	m_collisionRect;

	GLboolean	m_isGravityApplied;
	GLboolean	m_isDrawable;
	GLboolean   m_isCollidable;
	// Render state
	Texture2D   m_sprite;// maybe should be spritesheet?
};

#endif