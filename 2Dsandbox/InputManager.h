#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GameWorld.h"
/****************************************************************
**********************SINGLETON CLASS****************************
****************************************************************/

class InputManager
{
public:
	static InputManager* s_pInstance;

	static GLboolean initialize();
	static InputManager& instance();
	
	void Update(GLfloat detaTime);
	void OnKeyCallback(int key, int action);
	void OnMouseCallback(int button, int action, float xpos, float ypos);
	void mousePressedLeftAtPos(float x, float y);
	void mousePressedRightAtPos(float x, float y);
	
	GLboolean isKeyDown(int key);

	GLboolean	Keys[1024] = { false };

private:

	InputManager();
	~InputManager();
};

// Inline methods
//-------------------------------------------------------------------------- //
inline InputManager& InputManager::instance()
{
	return *s_pInstance;
}

#endif