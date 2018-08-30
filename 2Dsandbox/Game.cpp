#include "Game.h"

SpriteRenderer *Renderer;

Game::Game(GLuint width, GLuint height)
	:State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
	m_camera.Init(Width, Height);
}

Game::~Game()
{
	delete Renderer;
}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("Shaders/Sprite.vs", "Shaders/Sprite.fs", nullptr, ResourceManager::SPRITE_SHADER);
	// Configure shaders
	ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).Use().SetMatrix4("projection", m_camera.getCameraMatrix());
	// Load spritesheet and spritesheet params (uv rects)
	ResourceManager::LoadSpritesheet("textures/Tiles/tiles_spritesheet.png", GL_FALSE, ResourceManager::TILES_SPRITESHEET);
	ResourceManager::LoadSpritesheetParams("textures/Tiles/tiles_spritesheet.xml", ResourceManager::TILES_SPRITESHEET);
	// Load world
	//m_world.Load("WorldMaps/World1.map", this->Width, this->Height);
	m_world.Generate(3000, 1500);

	// Set render-specific controls
	Shader shader = ResourceManager::GetShader(ResourceManager::SPRITE_SHADER);
	Renderer = new SpriteRenderer(shader);
}

void Game::ProcessInput(GLfloat dt)
{
	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;
	if (this->State == GAME_ACTIVE)
	{		
		// Move camera
		if (this->Keys[GLFW_KEY_A])
		{
			m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
			ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetMatrix4("projection", m_camera.getCameraMatrix());
		}
		if (this->Keys[GLFW_KEY_D])
		{
			m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
			ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetMatrix4("projection", m_camera.getCameraMatrix());
		}
		if (this->Keys[GLFW_KEY_W])
		{
			m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
			ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetMatrix4("projection", m_camera.getCameraMatrix());
		}
		if (this->Keys[GLFW_KEY_S])
		{
			m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
			ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetMatrix4("projection", m_camera.getCameraMatrix());
		}
		if (this->Keys[GLFW_KEY_Q])
		{
			m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
			ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetMatrix4("projection", m_camera.getCameraMatrix());
		}
		if (this->Keys[GLFW_KEY_E])
		{
			m_camera.setScale(m_camera.getScale() - SCALE_SPEED);
			ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetMatrix4("projection", m_camera.getCameraMatrix());
		}
	}
}

void Game::Update(GLfloat dt)
{
	m_camera.Update();
}

void Game::Render(GLFWwindow* window)
{
	if (this->State == GAME_ACTIVE)
	{	
		//Set Base depth to 1
		glClearDepth(1.0f);
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);

		//ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).Use();
		glm::mat4 model = glm::mat4(1.0);
		ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetMatrix4("model", model);
		ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetMatrix4("projection", m_camera.getCameraMatrix());
		ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetInteger("image", 0);
		
		// Draw world tiles
		m_world.DrawInBatches(*Renderer, m_camera);

		//unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);
		/**************/
		glfwSwapBuffers(window);
	}
}

void Game::mousePressedAtPos(float x, float y)
{
	m_world.setTileAtPos(x + m_camera.getPosition().x, y + m_camera.getPosition().y, ResourceManager::EMPTY_TILE);
}
