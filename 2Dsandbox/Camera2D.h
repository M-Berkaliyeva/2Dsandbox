#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera2D
{
public:
	Camera2D();
	~Camera2D();

	void Init(GLuint screenWidth, GLuint screenHeight);
	void Update();

	void move(glm::vec2 dir);
	void zoom(GLfloat new_scale);
	
	// Setters
	void setCameraLimits(GLfloat limX, GLfloat limY) { m_camPosLimit = glm::vec2(limX - m_screenWidth, limY - m_screenHeight); }

	// Getters
	glm::vec2 getPosition() { return glm::vec2(m_cameraPos.x, m_cameraPos.y); }
	glm::vec2 getDimensions() { return glm::vec2(m_screenWidth, m_screenHeight); }
	float getScale() { return m_scale; }
	glm::mat4 getCameraMatrix() { return m_cameraMatrix; }
	glm::mat4 getViewMatrix() { return m_viewMatrix; }

private:
	GLuint m_screenWidth, m_screenHeight;
	bool m_needsMatrixUpdate;
	GLfloat m_scale;
	GLfloat m_speed;
	glm::vec2 m_camPosLimit;
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_orthoMatrix;
};