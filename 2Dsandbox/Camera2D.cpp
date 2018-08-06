#include "Camera2D.h"



Camera2D::Camera2D(): 
	m_position(0.0f, 0.0f),
	m_cameraMatrix(1.0f),
	m_orthoMatrix(1.0f),
	m_scale(1.0f), 
	m_needsMatrixUpdate(true),
	m_screenWidth(500),
	m_screenHeight(500)
{
}


Camera2D::~Camera2D()
{
}

void Camera2D::Init(GLuint screenWidth, GLuint screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_cameraMatrix = glm::ortho(0.0f, (float)m_screenWidth, (float)m_screenHeight, 0.0f, -1.0f, 1.0f);
}

void Camera2D::Update()
{
	if (m_needsMatrixUpdate)
	{
		// Camera translation
		glm::vec3 translate(-m_position.x , -m_position.y , 0.0f);
		m_cameraMatrix = glm::translate(m_cameraMatrix, translate);

		// Camera scaling
		glm::vec3 scale(m_scale, m_scale, 0.0f);
		m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

		m_needsMatrixUpdate = false;
	}
}
