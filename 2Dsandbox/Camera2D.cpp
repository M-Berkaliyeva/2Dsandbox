#include "Camera2D.h"

//-------------------------------------------------------------------------- //
Camera2D* Camera2D::s_pInstance = NULL;
//-------------------------------------------------------------------------- //

GLboolean Camera2D::initialize(GLuint screenWidth, GLuint screenHeight)
{
	if (s_pInstance)
		return GL_FALSE;

	s_pInstance = new Camera2D();

	if (!s_pInstance)
		return GL_FALSE;

	s_pInstance->Init(screenWidth, screenHeight);
	return GL_TRUE;
}

Camera2D::Camera2D(): 
	m_cameraPos(0.0f, 0.0f, 1.0f),
	m_cameraFront(0.0f, 0.0f, -1.0f),
	m_cameraUp(0.0f, 1.0f, 0.0f),
	m_camPosLimit(500.0f, 500.0f),
	m_camOffset(0.0f, 0.0f),
	m_viewMatrix(1.0f),
	m_cameraMatrix(1.0f),
	m_orthoMatrix(1.0f),
	m_scale(1.0f), 
	m_speed(10.0f),
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
	m_camOffset.x = screenWidth / 2;
	m_camOffset.y = screenHeight / 2;
	m_cameraMatrix = glm::ortho(0.0f, (float)m_screenWidth, (float)m_screenHeight, 0.0f, -1.0f, 1.0f);	
	m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void Camera2D::Update()
{
	//if (m_needsMatrixUpdate)
	//{
		// Camera translation
		//glm::vec3 translate(-m_position.x , -m_position.y , 0.0f);
		//m_cameraMatrix = glm::translate(m_cameraMatrix, translate);

		// Camera scaling
		//glm::vec3 scale(m_scale, m_scale, 0.0f);
		//m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

		//m_needsMatrixUpdate = false;
	//}
}

void Camera2D::move(glm::vec2 speed, GLfloat dt)
{
	glm::vec2 newPos = (glm::vec2(m_cameraPos.x, m_cameraPos.y) + speed * dt) - m_camOffset;
	
	//Check that we don't move camera outside world boundaries
	if (newPos.x >= 0 && newPos.x < m_camPosLimit.x)
	{
		m_cameraPos = glm::vec3(newPos.x, m_cameraPos.y, 1.0f);
		m_viewMatrix = glm::lookAt(m_cameraPos + m_cameraFront, m_cameraPos + m_cameraFront, m_cameraUp);
	}
	if (newPos.y >= 0 && newPos.y < m_camPosLimit.y)
	{
		m_cameraPos = glm::vec3(m_cameraPos.x, newPos.y, 1.0f);
		m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
	}
}

void Camera2D::moveTo(glm::vec2 pos)
{
	// Offset from player position
	glm::vec2 newPos = pos - m_camOffset;// -m_camOffset;

	//Check that we don't move camera outside world boundaries
	if (newPos.x >= 0 && newPos.x < m_camPosLimit.x)
	{
		m_cameraPos = glm::vec3(static_cast<int>(newPos.x), static_cast<int>(m_cameraPos.y), 1.0f);
		m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
	}
	if (newPos.y >= 0 && newPos.y < m_camPosLimit.y)
	{
		m_cameraPos = glm::vec3(static_cast<int>(m_cameraPos.x), static_cast<int>(newPos.y), 1.0f);
		m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
	}
	
}

void Camera2D::zoom(GLfloat new_scale)
{
	m_scale += new_scale;
	glm::vec3 scale(m_scale, m_scale, 0.0f);
	m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;
	//m_cameraPos.z += new_scale;
	//m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}
