#include "Game.h"

SpriteRenderer *Renderer;

Game::Game(GLuint width, GLuint height)
	:State(GAME_ACTIVE), Width(width), Height(height)
{
	
}

Game::~Game()
{
	delete Renderer;
}

void Game::Init()
{
	GameWorld &gameWorld = GameWorld::instance();
	Camera2D &camera = Camera2D::instance();
	InitShaders();
	InitTextures();
	m_background.OnLoad(Width, Height);
	// Load world
	gameWorld.CreateMap(300, 150);
	// Create player
	m_player.OnLoad(glm::vec2(Width/2, 432.0f));
	// Setup camera
	camera.setCameraLimits(gameWorld.m_worldWidthInPixels, gameWorld.m_worldHeightInPixels);//set camera move limits
	// Set render-specific controls	
	m_batchRenderer = new SpriteBatchRenderer();
	m_batchRenderer->init();
}

void Game::Update(GLfloat dt)
{
	m_gameTime = glfwGetTime() / 10;
	InputManager::instance().Update(dt);
	m_background.Update(dt, m_gameTime);
	m_player.Update(dt);
	// Should i do this here?
	Camera2D::instance().moveTo(m_player.GetPosition());
}

void Game::Render(GLFWwindow* window)
{	
	if (this->State == GAME_ACTIVE)
	{	
		//Set Base depth to 1
		glClearDepth(1.0f);
		glClearColor( 0.73f, 0.86f, 0.99f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
		//Start batching sprites
		m_batchRenderer->begin(GlyphSortType::BACK_TO_FRONT);
		
		//Add Background layers to batch
		m_background.Draw(*m_batchRenderer);
		// Add world tiles to batch
		GameWorld::instance().DrawInBatches(*m_batchRenderer);
		// Add playerto batch
		m_player.Draw(*m_batchRenderer);

		// Render batch
		m_batchRenderer->end();
		m_batchRenderer->renderBatch();
		glBindVertexArray(0);
		/**************/
		glfwSwapBuffers(window);
	}
}

//void Game::mousePressedLeftAtPos(float x, float y)
//{
	//m_world.setTileAtPos(x + m_camera.getPosition().x, y + m_camera.getPosition().y, ResourceManager::EMPTY_TILE);
//}

//void Game::mousePressedRightAtPos(float x, float y)
//{
	//GLfloat posX = x + m_camera.getPosition().x;
	//GLfloat posY = y + m_camera.getPosition().y;
	//if(m_world.getTileAtPos(posX, posY) == ResourceManager::EMPTY_TILE)
	//	m_world.setTileAtPos(posX, posY, ResourceManager::GRASS_TILE);
//}

void Game::InitShaders()
{
	Camera2D &camera = Camera2D::instance();
	// Load shaders
	ResourceManager::LoadShader("Shaders/TexturedSpriteShader.vs", "Shaders/TexturedSpriteShader.fs", nullptr, ResourceManager::TEXTURED_SPRITE_SHADER);
	ResourceManager::LoadShader("Shaders/SkyBackgroundShader.vs", "Shaders/SkyBackgroundShader.fs", nullptr, ResourceManager::SKY_BACKGROUND_SHADER);
	ResourceManager::LoadShader("Shaders/OverlayBackgroundShader.vs", "Shaders/OverlayBackgroundShader.fs", nullptr, ResourceManager::OVERLAY_BACKGROUND_SHADER);
	// Configure shaders
	glm::mat4 model = glm::mat4(1.0);
	ResourceManager::GetShader(ResourceManager::TEXTURED_SPRITE_SHADER)->SetInteger("image", 0, true);
	ResourceManager::GetShader(ResourceManager::TEXTURED_SPRITE_SHADER)->SetMatrix4("projection", camera.getCameraMatrix(), true);
	ResourceManager::GetShader(ResourceManager::TEXTURED_SPRITE_SHADER)->SetMatrix4("view", camera.getViewMatrix(), true);
	ResourceManager::GetShader(ResourceManager::TEXTURED_SPRITE_SHADER)->SetMatrix4("model", model, true);

	//ResourceManager::GetShader(ResourceManager::SKY_BACKGROUND_SHADER)->SetInteger("image", 0, true);
	ResourceManager::GetShader(ResourceManager::SKY_BACKGROUND_SHADER)->SetMatrix4("projection", camera.getCameraMatrix(), true);
	ResourceManager::GetShader(ResourceManager::SKY_BACKGROUND_SHADER)->SetMatrix4("view", camera.getViewMatrix(), true);
	ResourceManager::GetShader(ResourceManager::SKY_BACKGROUND_SHADER)->SetMatrix4("model", model, true);
	ResourceManager::GetShader(ResourceManager::SKY_BACKGROUND_SHADER)->SetFloat("time", 0.0f, true);

	ResourceManager::GetShader(ResourceManager::OVERLAY_BACKGROUND_SHADER)->SetMatrix4("projection", camera.getCameraMatrix(), true);
	ResourceManager::GetShader(ResourceManager::OVERLAY_BACKGROUND_SHADER)->SetMatrix4("view", camera.getViewMatrix(), true);
	ResourceManager::GetShader(ResourceManager::OVERLAY_BACKGROUND_SHADER)->SetMatrix4("model", model, true);
	ResourceManager::GetShader(ResourceManager::OVERLAY_BACKGROUND_SHADER)->SetFloat("time", 0.0f, true);

}

void Game::InitTextures()
{
	// Load spritesheet and spritesheet params (uv rects)
	ResourceManager::LoadTextures("textures/Player/PlayerSpritesheet.png", GL_FALSE, ResourceManager::PLAYER_TILESHEET, glm::ivec2(16, 1));
	ResourceManager::LoadTextures("textures/Tiles/tiles_spritesheet.png", GL_FALSE, ResourceManager::TILES_TILESHEET, glm::ivec2(70, 70));
	ResourceManager::LoadTextures("textures/Backgrounds/clouds_BG.png", GL_FALSE, ResourceManager::CLOUDS_BG_BACKGROUND_TEXTURE, glm::ivec2(1, 1));
	ResourceManager::LoadTextures("textures/Backgrounds/mountains.png", GL_FALSE, ResourceManager::MOUNTAINS_BACKGROUND_TEXTURE, glm::ivec2(1, 1));
	ResourceManager::LoadTextures("textures/Backgrounds/clouds_FG.png", GL_FALSE, ResourceManager::CLOUDS_FG_BACKGROUND_TEXTURE, glm::ivec2(1, 1));
	ResourceManager::LoadSpritesheetParams("textures/Tiles/tiles_spritesheet2.xml", ResourceManager::TILES_TILESHEET);
}
