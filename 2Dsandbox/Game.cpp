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
	InitShaders();
	InitTextures();
	// Load world
	m_world.CreateMap(300, 150);
	// Create player
	m_player = new GameObject(GameObject::OBJECT_TYPE_PLAYER, glm::vec2(Width/2, Height/2), glm::vec2(32.0f, 48.0f), ResourceManager::GetSpritesheet(ResourceManager::PLAYER_SPRITESHEET));
	// Setup camera
	m_camera.setCameraLimits(m_world.m_worldWidthInPixels, m_world.m_worldHeightInPixels);//set camera move limits
	// Set render-specific controls
	//Shader shader = ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER);
	//Renderer = new SpriteRenderer(shader);
	m_batchRenderer = new SpriteBatchRenderer();
	m_batchRenderer->init();
}

void Game::ProcessInput(GLfloat dt)
{
	const float CAMERA_SPEED = 1.0f;
	const float SCALE_SPEED = 0.1f;
	if (this->State == GAME_ACTIVE)
	{		
		// Move camera
		if (this->Keys[GLFW_KEY_A])
		{
			m_camera.move(glm::vec2(-1.0f, 0.0f));	
		}
		if (this->Keys[GLFW_KEY_D])
		{
			m_camera.move(glm::vec2(1.0f, 0.0f));
		}
		if (this->Keys[GLFW_KEY_W])
		{
			m_camera.move(glm::vec2(0.0f, 1.0f));
		}
		if (this->Keys[GLFW_KEY_S])
		{
			m_camera.move(glm::vec2(0.0f, -1.0f));
		}
		if (this->Keys[GLFW_KEY_Q])
		{
			m_camera.zoom(SCALE_SPEED);// setScale(m_camera.getScale() + SCALE_SPEED);
		}
		if (this->Keys[GLFW_KEY_E])
		{
			m_camera.zoom(-SCALE_SPEED);// setScale(m_camera.getScale() - SCALE_SPEED);
		}
	}
}

void Game::Update(GLfloat dt)
{
	m_camera.Update();
	m_player->Update();
}

void Game::Render(GLFWwindow* window)
{
	if (this->State == GAME_ACTIVE)
	{	
		//Set Base depth to 1
		glClearDepth(1.0f);
		glClearColor( 0.73f, 0.86f, 0.99f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);

		//ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).Use();
		
		//ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetMatrix4("projection", m_camera.getCameraMatrix());
		//ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).SetInteger("image", 0);
		
		//Start batching sprites
		m_batchRenderer->begin(GlyphSortType::BACK_TO_FRONT);
		// Draw world tiles
		m_world.DrawInBatches(*m_batchRenderer, m_camera);
		//Draw player
		m_player->Draw(*m_batchRenderer);
		m_batchRenderer->end();
		m_batchRenderer->renderBatch();

		//m_world.DrawInBatches(*Renderer, m_camera);
		//m_player->Draw(*Renderer);
		//unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);
		/**************/
		glfwSwapBuffers(window);
	}
}

void Game::mousePressedLeftAtPos(float x, float y)
{
	m_world.setTileAtPos(x + m_camera.getPosition().x, y + m_camera.getPosition().y, ResourceManager::EMPTY_TILE);
}

void Game::mousePressedRightAtPos(float x, float y)
{
	GLfloat posX = x + m_camera.getPosition().x;
	GLfloat posY = y + m_camera.getPosition().y;
	if(m_world.getTileAtPos(posX, posY) == ResourceManager::EMPTY_TILE)
		m_world.setTileAtPos(posX, posY, ResourceManager::GRASS_TILE);
}

void Game::InitShaders()
{
	// Load shaders
	ResourceManager::LoadShader("Shaders/TileShader.vs", "Shaders/TileShader.fs", nullptr, ResourceManager::TILE_SHADER);
	ResourceManager::LoadShader("Shaders/GameObject.vs", "Shaders/GameObject.fs", nullptr, ResourceManager::GAMEOBJECT_SHADER);
	// Configure shaders
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).Use().SetMatrix4("projection", m_camera.getCameraMatrix());
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).Use().SetMatrix4("view", m_camera.getViewMatrix());

	ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER).Use().SetMatrix4("projection", m_camera.getCameraMatrix());
	ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER).Use().SetMatrix4("view", m_camera.getViewMatrix());

}

void Game::InitTextures()
{
	// Load spritesheet and spritesheet params (uv rects)
	ResourceManager::LoadSpritesheet("textures/Player/p1_jump.png", GL_FALSE, ResourceManager::PLAYER_SPRITESHEET);
	ResourceManager::LoadSpritesheet("textures/Tiles/tiles_spritesheet.png", GL_FALSE, ResourceManager::TILES_SPRITESHEET);
	ResourceManager::LoadSpritesheetParams("textures/Tiles/tiles_spritesheet2.xml", ResourceManager::TILES_SPRITESHEET);
}
