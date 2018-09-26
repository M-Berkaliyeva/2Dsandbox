#include "GameObject.h"

const float GameObject::GRAVITY = 0.4f;

GameObject::GameObject()
	: m_objectType(OBJECT_TYPE_GENERIC),
	m_position(0.0f, 0.0f), 
	m_size(0.0f, 0.0f), 
	m_speed(0.0f, 0.0f), 
	m_acceleration(0.0f, 0.0f),
	m_collisionRect(0.0f, 0.0f, 0.0f, 0.0f),
	m_isGravityApplied(false),
	m_isDrawable(false),
	m_isCollidable(false),
	m_sprite()
{ }

GameObject::GameObject(ObjectType objType, glm::vec2 pos, glm::vec2 size, Texture2D sprite, GLboolean isCollidable, GLboolean isGravityApplied)
	: m_objectType(objType),
	m_position(pos),
	m_size(size), 
	m_speed(0.0f, 0.0f),
	m_acceleration(0.0f, 0.0f),
	m_collisionRect(0.0f, 0.0f, 0.0f, 0.0f),
	m_sprite(sprite),
	m_isCollidable(isCollidable),
	m_isGravityApplied(isGravityApplied)
{ }

void GameObject::Update()
{
	if (m_isGravityApplied)
	{
		m_acceleration.y = GRAVITY;
	}

	m_speed += m_acceleration;

	//should keep speed at bounds so dont accelerate forever (need max speed )

	Move(m_speed);
}

void GameObject::Draw(SpriteBatchRenderer &renderer)
{	
	glm::vec4 pos(m_position.x, m_position.y, m_size.x, m_size.y);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Color col; col.r = 255; col.g = 255; col.b = 255; col.a = 255;
	renderer.draw(pos, uv, m_sprite.ID, -1.0f, col);
}

void GameObject::OnCollision(GameObject * otherObject)
{
}

void GameObject::Move(glm::vec2 speed)
{
	m_position += m_speed;
}

bool GameObject::Collides(glm::vec4 otherCollisionRect)
{
	GLint myLeft, otherLeft;
	GLint myRight, otherRight;
	GLint myTop, otherTop;
	GLint myBottom, otherBottom;

	myLeft = (GLint) m_position.x + (GLint)m_collisionRect.x;
	otherLeft = otherCollisionRect.x;

	myRight = myLeft + (GLint)m_collisionRect.z;
	otherRight = otherCollisionRect.x + otherCollisionRect.z;

	myTop = (GLint)m_position.y + (GLint)m_collisionRect.y;
	otherTop = otherCollisionRect.y;

	myBottom = myTop + (GLint)m_collisionRect.w;
	otherBottom = otherCollisionRect.y + otherCollisionRect.w;

	if (myBottom < otherTop) return false;
	if (myTop > otherBottom) return false;

	if (myRight < otherLeft) return false;
	if (myLeft > otherRight) return false;

	return true;
}
