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

	// Setters
	void setPosition(const glm::vec2& newPosition) { m_position = newPosition; m_needsMatrixUpdate = true; }
	void setScale(GLfloat newScale) { m_scale = newScale; m_needsMatrixUpdate = true; }

	// Getters
	glm::vec2 getPosition() { return m_position; }
	float getScale() { return m_scale; }
	glm::mat4 getCameraMatrix() { return m_cameraMatrix; }

private:
	GLuint m_screenWidth, m_screenHeight;
	bool m_needsMatrixUpdate;
	GLfloat m_scale;
	glm::vec2 m_position;
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_orthoMatrix;

};

