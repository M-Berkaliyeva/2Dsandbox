#include "GameObject.h"

const float GameObject::GRAVITY = 19.8f;

GameObject::GameObject()
{
	m_objectType = OBJECT_TYPE_GENERIC;
	m_position = glm::vec2(0.0f, 0.0f);
	m_size = glm::vec2(0.0f, 0.0f);
	m_speed = glm::vec2(0.0f, 0.0f);
	m_acceleration = glm::vec2(0.0f, 0.0f);
	m_collisionRect = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	m_isCollidable = false;
	m_isGravityApplied = false;
	m_isGrounded = false;
	m_wasGrounded = false;
	m_isHittingCeiling = false;
	m_wasHittingCeiling = false;
	m_isHittingLeftWall = false;
	m_wasHittingLeftWall = false;
	m_isHittingRightWall = false;
	m_wasHittingRightWall = false;
}

void GameObject::OnLoad()
{

}

void GameObject::Update(GLfloat deltaTime)
{
	// Set previous frame's data
	m_prevPosition = m_position;
	m_prevSpeed = m_speed;

	m_wasGrounded = m_isGrounded;
	m_wasHittingCeiling = m_isHittingCeiling;
	m_wasHittingLeftWall = m_isHittingLeftWall;
	m_wasHittingRightWall = m_isHittingRightWall;
	// need these for other entities than player?
	if (m_objectType != OBJECT_TYPE_PLAYER && m_isGravityApplied)
	{
		m_acceleration.y = GRAVITY;
	}

	m_speed += m_acceleration;

	CheckCollisionWithMap();

	Move(deltaTime);
}

/*****************************************************************************************************
* Checks if there is any collision with ground ceiling walls, if yes pushes out of the collision tile
*****************************************************************************************************/
void GameObject::CheckCollisionWithMap()
{
	float groundY = 0.0f, ceilingY = 0.0f;
	float rightWallX = 0.0f, leftWallX = 0.0f;

	// Check collision with left wall
	if (m_speed.x <= 0.0f
		&& HasWallOnLeft(m_prevPosition, m_position, leftWallX))
	{
		if (m_prevPosition.x - m_collisionRect.z + m_collisionRect.x >= leftWallX)
		{
			m_position.x = leftWallX + m_collisionRect.z - m_collisionRect.x;
			m_isHittingLeftWall = true;
		}
		m_speed.x = std::fmaxf(m_speed.x, 0.0f);
		//m_speed.x = 0.0f;
	}
	else
	{
		m_isHittingLeftWall = false;
	}

	//Check collision with right wall
	if (m_speed.x >= 0.0f
		&& HasWallOnRight(m_prevPosition, m_position, rightWallX))
	{
		if (m_prevPosition.x + m_collisionRect.z + m_collisionRect.x <= rightWallX)
		{
			m_position.x = rightWallX - m_collisionRect.z - m_collisionRect.x;
			m_isHittingRightWall = true;
		}
		m_speed.x = std::fminf(m_speed.x, 0.0f);
		//m_speed.x = 0.0f;
	}
	else
	{
		m_isHittingRightWall = false;
	}

	//Check collision with ground
	if (m_speed.y >= 0.0f && HasGround(m_prevPosition, m_position, m_speed, groundY))
	{
		m_position.y = groundY - m_collisionRect.w - m_collisionRect.y;
		m_speed.y = 0.0f;
		m_isGrounded = true;
	}
	else
		m_isGrounded = false;

	//Check collision with ceiling
	if (m_speed.y <= 0.0f && HasCeiling(m_prevPosition, m_position, ceilingY))
	{
		m_position.y = ceilingY + 1.0f;
		m_speed.y = 0.0f;
		m_isHittingCeiling = true;
	}
	else
		m_isHittingCeiling = false;
}

void GameObject::RoundVector(glm::vec2 & vecToRound)
{
	vecToRound = glm::vec2(roundf(vecToRound.x), roundf(vecToRound.y));
}

void GameObject::Draw(SpriteBatchRenderer &renderer)
{
	Camera2D &cam = Camera2D::instance();
	glm::vec2 pos1(m_position.x - cam.getPosition().x, m_position.y - cam.getPosition().y);
	RoundVector(pos1);
	glm::vec4 pos(pos1.x, pos1.y, m_size.x, m_size.y);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Color col; col.r = 255; col.g = 255; col.b = 255; col.a = 255;
	renderer.draw(pos, uv, m_sprite.ID, -1.0f, col);
}

void GameObject::OnCollision(GameObject * otherObject)
{
}

void GameObject::Move(GLfloat deltaTime)
{
	GameWorld &map = GameWorld::instance();
	glm::ivec2 newPos = m_position + m_speed * deltaTime;
	// Don't move outside the world
	if (newPos.x >= 0 && newPos.x < map.m_worldWidthInPixels)
	{
		m_position.x = newPos.x;
	}
	if (newPos.y >= 0 && newPos.y < map.m_worldHeightInPixels)
	{
		m_position.y = newPos.y;
	}
}

bool GameObject::Collides(glm::vec4 otherCollisionRect)
{
	GLint myLeft, otherLeft;
	GLint myRight, otherRight;
	GLint myTop, otherTop;
	GLint myBottom, otherBottom;

	myLeft = (GLint)m_position.x + (GLint)m_collisionRect.x;
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

/*************************************************************
*Returns true if object overlaps with any of the bottom tiles
**************************************************************/
GLboolean GameObject::HasGround(glm::vec2 oldPosition, glm::vec2 newPosition, glm::vec2 speed, float & OUT_groundY)
{
	GameWorld &map = GameWorld::instance();
	Camera2D &cam = Camera2D::instance();
	// Find out bottom line of this object
	//Bottom left = pos + offset + collisionRect height
	glm::vec2 bottomLeftPoint = newPosition + glm::vec2(m_collisionRect.x, m_collisionRect.y + m_collisionRect.w);
	glm::vec2 bottomRightPoint = newPosition + glm::vec2(m_collisionRect.x + m_collisionRect.z, m_collisionRect.y + m_collisionRect.w);

	RoundVector(bottomLeftPoint);
	RoundVector(bottomRightPoint);

	//Now calculate which tiles  we need to check
	glm::ivec2 tileIndex;
	for (glm::vec2 checkedTile = bottomLeftPoint; ; checkedTile.x += map.m_tileWidth)
	{
		checkedTile.x = std::fminf(checkedTile.x, bottomRightPoint.x);
		tileIndex = map.getTileIndexAtPos(checkedTile.x, checkedTile.y);

		// Calculate tile's top position
		OUT_groundY = (float)tileIndex.y * map.m_tileHeight;// + cam.getPosition().y;

		// If current tile checed is solid (obstacle) we are colliding -> return true
		if (map.isSolidTile(tileIndex.x, tileIndex.y))
			return true;

		//Check if we reached the end of sensor line -> exit the loop
		if (checkedTile.x >= bottomRightPoint.x)
			break;
	}

	// If we didn't hit any obstacles by now -> return false
	return false;
}

/*************************************************************
*Returns true if object overlaps with any of the top tiles
**************************************************************/
GLboolean GameObject::HasCeiling(glm::vec2 oldPosition, glm::vec2 newPosition, float & ceilingY)
{
	GameWorld &map = GameWorld::instance();
	Camera2D &cam = Camera2D::instance();

	ceilingY = 0.0f;
	// Find out top line of this object
	//Top left = pos + offset, Top Right = pos + offset + collisionRect Width
	glm::vec2 topLeftPoint = newPosition + glm::vec2(m_collisionRect.x, m_collisionRect.y);
	glm::vec2 topRightPoint = newPosition + glm::vec2(m_collisionRect.x + m_collisionRect.z, m_collisionRect.y);

	RoundVector(topLeftPoint);
	RoundVector(topRightPoint);

	//Now calculate which tiles  we need to check
	glm::ivec2 tileIndex;
	for (glm::vec2 checkedTile = topLeftPoint; ; checkedTile.x += map.m_tileWidth)
	{
		checkedTile.x = std::fminf(checkedTile.x, topRightPoint.x);
		tileIndex = map.getTileIndexAtPos(checkedTile.x, checkedTile.y);

		// If current tile checed is solid (obstacle) we are colliding -> return true
		if (map.isSolidTile(tileIndex.x, tileIndex.y))
		{
			// Calculate tile's bottom position
			ceilingY = tileIndex.y * map.m_tileHeight + map.m_tileHeight;// +cam.getPosition().y;
			return true;
		}

		//Check if we reached the end of sensor line -> exit the loop
		if (checkedTile.x >= topRightPoint.x)
			break;
	}

	// If we didn't hit any obstacles by now -> return false
	return false;
}

GLboolean GameObject::HasWallOnLeft(glm::vec2 oldPosition, glm::vec2 newPosition, float & wallX)
{
	GameWorld &map = GameWorld::instance();
	Camera2D &cam = Camera2D::instance();

	wallX = 0.0f;
	// Find out left sensor line of this object -  1 pixel to the left
	//Top left = pos + offset, bottom left = pos + offset + collisionRect Height
	glm::vec2 topLeftPoint = newPosition + glm::vec2(m_collisionRect.x, m_collisionRect.y);
	glm::vec2 bottomLeftPoint = newPosition + glm::vec2(m_collisionRect.x, m_collisionRect.y + m_collisionRect.w);

	RoundVector(topLeftPoint);
	RoundVector(bottomLeftPoint);

	//Now calculate which tiles  we need to check
	glm::ivec2 tileIndex;
	for (glm::vec2 checkedTile = topLeftPoint; ; checkedTile.y += map.m_tileHeight)
	{
		checkedTile.y = std::fminf(checkedTile.y, bottomLeftPoint.y);
		tileIndex = map.getTileIndexAtPos(checkedTile.x, checkedTile.y);

		//Check if we reached the end of sensor line -> exit the loop
		if (checkedTile.y >= bottomLeftPoint.y)
			break;

		// If current tile checed is solid (obstacle) we are colliding -> return true
		if (map.isSolidTile(tileIndex.x, tileIndex.y))
		{
			// Calculate tile's right position
			wallX = (float)tileIndex.x * map.m_tileWidth + map.m_tileWidth;// +cam.getPosition().x;
			return true;
		}
	}

	// If we didn't hit any obstacles by now -> return false
	return false;
}

GLboolean GameObject::HasWallOnRight(glm::vec2 oldPosition, glm::vec2 newPosition, float & wallX)
{
	GameWorld &map = GameWorld::instance();
	Camera2D &cam = Camera2D::instance();

	wallX = 0.0f;
	// Find out left line of this object
	//Top left = pos + offset, bottom left = pos + offset + collisionRect Height
	glm::vec2 topRightPoint = newPosition + glm::vec2(m_collisionRect.x + m_collisionRect.z, m_collisionRect.y);
	glm::vec2 bottomRightPoint = newPosition + glm::vec2(m_collisionRect.x + m_collisionRect.z, m_collisionRect.y + m_collisionRect.w);

	RoundVector(topRightPoint);
	RoundVector(bottomRightPoint);

	//Now calculate which tiles  we need to check
	glm::ivec2 tileIndex;
	for (glm::vec2 checkedTile = topRightPoint; ; checkedTile.y += map.m_tileHeight)
	{
		checkedTile.y = std::fminf(checkedTile.y, bottomRightPoint.y);
		tileIndex = map.getTileIndexAtPos(checkedTile.x, checkedTile.y);

		//Check if we reached the end of sensor line -> exit the loop
		if (checkedTile.y >= bottomRightPoint.y)
			break;

		// If current tile checed is solid (obstacle) we are colliding -> return true
		if (map.isSolidTile(tileIndex.x, tileIndex.y))
		{
			// Calculate tile's left position
			GLfloat camPos = cam.getPosition().x;
			wallX = (float)tileIndex.x * map.m_tileWidth - map.m_tileWidth;// +camPos;
			return true;
		}
	}

	// If we didn't hit any obstacles by now -> return false
	return false;
}
