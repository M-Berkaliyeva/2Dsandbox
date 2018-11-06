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
	m_bgSprite = ResourceManager::GetSpritesheet(ResourceManager::BACKGROUND_SPRITESHEET);
	m_bgUVu = 0.0f;
	m_bgspeed = 1.0f;
	// Load world
	gameWorld.CreateMap(300, 150);
	// Create player
	m_player.OnLoad(glm::vec2(Width/2, 432.0f), ResourceManager::GetSpritesheet(ResourceManager::PLAYER_SPRITESHEET));
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
	
	ResourceManager::GetShader(ResourceManager::BACKGROUND_SHADER).SetFloat("time", glfwGetTime()/10, true);
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
		
		glActiveTexture(GL_TEXTURE0);
		
		//Start batching sprites
		m_batchRenderer->begin(GlyphSortType::BACK_TO_FRONT);
		// Add background to btch

		glm::vec4 pos(0.0f, 0.0f, Width, Height);
		glm::vec4 uv(0.0f + m_bgspeed, 0.0f, 1.0f, 1.0f);
		Color col; col.r = 255; col.g = 255; col.b = 255; col.a = 255;
		m_batchRenderer->draw(pos, uv, m_bgSprite.ID, ResourceManager::GetShader(ResourceManager::BACKGROUND_SHADER).ID, 2.0f, col);
		// Add world tiles to batch
		GameWorld::instance().DrawInBatches(*m_batchRenderer);
		// Add playerto batch
		m_player.Draw(*m_batchRenderer);
		// Render batch
		m_batchRenderer->end();
		m_batchRenderer->renderBatch();

		//unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

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
	ResourceManager::LoadShader("Shaders/GameObjectShader.vs", "Shaders/GameObjectShader.fs", nullptr, ResourceManager::GAMEOBJECT_SHADER);
	ResourceManager::LoadShader("Shaders/BackgroundShader.vs", "Shaders/BackgroundShader.fs", nullptr, ResourceManager::BACKGROUND_SHADER);
	// Configure shaders
	glm::mat4 model = glm::mat4(1.0);
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).SetInteger("image", 0, true);
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).SetMatrix4("projection", camera.getCameraMatrix(), true);
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).SetMatrix4("view", camera.getViewMatrix(), true);
	ResourceManager::GetShader(ResourceManager::TILE_SHADER).SetMatrix4("model", model, true);

	ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER).SetInteger("image", 0, true);
	ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER).SetMatrix4("projection", camera.getCameraMatrix(), true);
	ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER).SetMatrix4("view", camera.getViewMatrix(), true);
	ResourceManager::GetShader(ResourceManager::GAMEOBJECT_SHADER).SetMatrix4("model", model, true);

	ResourceManager::GetShader(ResourceManager::BACKGROUND_SHADER).SetInteger("image", 0, true);
	ResourceManager::GetShader(ResourceManager::BACKGROUND_SHADER).SetMatrix4("projection", camera.getCameraMatrix(), true);
	ResourceManager::GetShader(ResourceManager::BACKGROUND_SHADER).SetMatrix4("view", camera.getViewMatrix(), true);
	ResourceManager::GetShader(ResourceManager::BACKGROUND_SHADER).SetMatrix4("model", model, true);
	ResourceManager::GetShader(ResourceManager::BACKGROUND_SHADER).SetFloat("time", 0.0f, true);

}

void Game::InitTextures()
{
	// Load spritesheet and spritesheet params (uv rects)
	ResourceManager::LoadSpritesheet("textures/Player/p1_jump.png", GL_FALSE, ResourceManager::PLAYER_SPRITESHEET);
	ResourceManager::LoadSpritesheet("textures/Tiles/tiles_spritesheet.png", GL_FALSE, ResourceManager::TILES_SPRITESHEET);
	ResourceManager::LoadSpritesheet("textures/Backgrounds/background.png", GL_FALSE, ResourceManager::BACKGROUND_SPRITESHEET);
	ResourceManager::LoadSpritesheetParams("textures/Tiles/tiles_spritesheet2.xml", ResourceManager::TILES_SPRITESHEET);
}
