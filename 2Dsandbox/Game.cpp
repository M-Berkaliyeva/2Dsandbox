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
	m_bgSprite = ResourceManager::GetTileSheet(ResourceManager::BACKGROUND_TILESHEET);
	m_bgUVu = 0.0f;
	m_bgspeed = 1.0f;
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
	InputManager::instance().Update(dt);

	m_player.Update(dt);
	// Should i do this here?
	Camera2D::instance().moveTo(m_player.GetPosition());
}

void Game::Render(GLFWwindow* window)
{
	m_bgspeed += 0.001;
	if (m_bgspeed > 1.0f)
		m_bgspeed = 0.0f;
	if (this->State == GAME_ACTIVE)
	{	
		//Set Base depth to 1
		glClearDepth(1.0f);
		glClearColor( 0.73f, 0.86f, 0.99f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//glActiveTexture(GL_TEXTURE0);
		
		//Start batching sprites
		m_batchRenderer->begin(GlyphSortType::BACK_TO_FRONT);
		// Add background to btch

		glm::vec4 pos(0.0f, 0.0f, Width, Height);
		glm::vec4 uv(0.0f + m_bgspeed, 0.0f, 1.0f, 1.0f);
		Color col; col.r = 255; col.g = 255; col.b = 255; col.a = 255;
		m_batchRenderer->draw(pos, uv, m_bgSprite.texture.ID, 2.0f, col);
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
	ResourceManager::LoadShader("Shaders/TileShader.vs", "Shaders/TileShader.fs", nullptr, ResourceManager::TILE_SHADER);
	ResourceManager::LoadShader("Shaders/GameObject.vs", "Shaders/GameObject.fs", nullptr, ResourceManager::GAMEOBJECT_SHADER);
	// Configure shaders
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).Use().SetMatrix4("projection", camera.getCameraMatrix());
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).Use().SetMatrix4("view", camera.getViewMatrix());

	ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER).Use().SetMatrix4("projection", camera.getCameraMatrix());
	ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER).Use().SetMatrix4("view", camera.getViewMatrix());

}

void Game::InitTextures()
{
	// Load spritesheet and spritesheet params (uv rects)
	ResourceManager::LoadTileSheet("textures/Player/PlayerSpritesheet.png", GL_FALSE, ResourceManager::PLAYER_TILESHEET, glm::ivec2(16, 1));
	ResourceManager::LoadTileSheet("textures/Tiles/tiles_spritesheet.png", GL_FALSE, ResourceManager::TILES_TILESHEET, glm::ivec2(70, 70));
	ResourceManager::LoadTileSheet("textures/Backgrounds/background.png", GL_FALSE, ResourceManager::BACKGROUND_TILESHEET, glm::ivec2(3020, 1760));
	ResourceManager::LoadSpritesheetParams("textures/Tiles/tiles_spritesheet2.xml", ResourceManager::TILES_TILESHEET);
}
