#include "InputManager.h"

//-------------------------------------------------------------------------- //
InputManager* InputManager::s_pInstance = NULL;
//-------------------------------------------------------------------------- //

GLboolean InputManager::initialize()
{
	if (s_pInstance)
		return GL_FALSE;

	s_pInstance = new InputManager();

	if (!s_pInstance)
		return GL_FALSE;

	//s_pInstance->initializeInternal();
	return GL_TRUE;
}

void InputManager::Update(GLfloat detaTime)
{
	
}

void InputManager::OnKeyCallback(int key, int action)
{
	if (action == GLFW_PRESS && !Keys[key])
		Keys[key] = GL_TRUE;
	else if (action == GLFW_RELEASE)
		Keys[key] = GL_FALSE;
}

void InputManager::OnMouseCallback(int button, int action, float xpos, float ypos)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		GameWorld::instance().setTileAtPos(xpos, ypos, ResourceManager::EMPTY_TILE);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		GLfloat posX = xpos;
		GLfloat posY = ypos;
		if(GameWorld::instance().getTileTypeAtPos(posX, posY) == ResourceManager::EMPTY_TILE)
			GameWorld::instance().setTileAtPos(posX, posY, ResourceManager::GRASS_TILE);
	}
}

GLboolean InputManager::isKeyDown(int key)
{
	return Keys[key];
}

InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}
