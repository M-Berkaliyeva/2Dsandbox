#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Game.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Camera2D.h"

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, int button, int action, int mods);

// Function to display FPS
void displayFPS(GLfloat deltaTime);

// The Width of the screen
const GLuint SCREEN_WIDTH = 1600;
// The height of the screen
const GLuint SCREEN_HEIGHT = 1200;

Game My2DSandbox(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "2DSandbox", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, cursor_position_callback);

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize game
	Camera2D::initialize(SCREEN_WIDTH, SCREEN_HEIGHT);
	InputManager::initialize();
	GameWorld::initialize();
	My2DSandbox.Init();

	// Semi fixed timestep: keep the upper bound for update rate (dt)
	// DeltaTime variables
	GLfloat frameTime = 0.0f;
	GLfloat dt = 1 / 60.0f;
	GLfloat lastFrame = glfwGetTime();

	// Start Game within Menu State
	My2DSandbox.State = Game::GAME_ACTIVE;
	
	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		frameTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		while (frameTime > 0.0f)
		{
			GLfloat deltaTime = std::fminf(frameTime, dt);
			//displayFPS(deltaTime);

			glfwPollEvents();
			// Update Game state
			My2DSandbox.Update(deltaTime);

			frameTime -= deltaTime;
		}
		// Render		
		My2DSandbox.Render(window);

		//glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using the resource manager
	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void displayFPS(GLfloat deltaTime)
{
	GLfloat frameCount = 1000 / deltaTime;
	std::cout << "FPS: " << frameCount;
	std::cout <<"\n";
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		InputManager::instance().OnKeyCallback(key, action);		
	}
}

void cursor_position_callback(GLFWwindow* window, int button, int action, int mods)
{
	if ( action == GLFW_PRESS)
	{
		double xpos, ypos;
		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;

		InputManager::instance().OnMouseCallback(button, action, (float)xpos, (float)ypos);				
	}
	
}

