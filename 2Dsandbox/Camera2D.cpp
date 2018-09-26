#include "Camera2D.h"



Camera2D::Camera2D(): 
	m_cameraPos(0.0f, 0.0f, 1.0f),
	m_cameraFront(0.0f, 0.0f, -1.0f),
	m_cameraUp(0.0f, 1.0f, 0.0f),
	m_camPosLimit(500.0f, 500.0f),
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

void Camera2D::move(glm::vec2 dir)
{
	/*m_position.x += m_speed * dir.x;
	m_position.y += m_speed * dir.y;
	glm::vec3 translate(-m_position.x, -m_position.y, 0.0f);
	m_cameraMatrix = glm::translate(m_cameraMatrix, translate);*/

	GLfloat newPosX = m_cameraPos.x + m_speed * dir.x;
	GLfloat newPosY = m_cameraPos.y - m_speed * dir.y;
	
	//Check that we don't move camera outside world boundaries
	if (newPosX >= 0 && newPosX < m_camPosLimit.x && newPosY >= 0 && newPosY < m_camPosLimit.y)
	{
		m_cameraPos.x = newPosX;
		m_cameraPos.y = newPosY;
		m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
	}	
}

void Camera2D::zoom(GLfloat new_scale)
{
	/*m_scale += new_scale;
	glm::vec3 scale(m_scale, m_scale, 0.0f);
	m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;*/
	//m_cameraPos.z += new_scale;
	//m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}
