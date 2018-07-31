#include "Game.h"

SpriteRenderer *Renderer;

Game::Game(GLuint width, GLuint height)
	:State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

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
	// TODO: here I also want to create camera matrix
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).Use().SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, ResourceManager::BACKGROUND_TEX);
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, ResourceManager::BLOCK_TEX);
	ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, ResourceManager::BLOCK_SOLID_TEX);
	ResourceManager::LoadTexture("textures/dirt.png", GL_FALSE, ResourceManager::DIRT_TEX);
	ResourceManager::LoadTexture("textures/grass.jpg", GL_FALSE, ResourceManager::GRASS_TEX);
	ResourceManager::LoadTexture("textures/grass.jpg", GL_FALSE, ResourceManager::STONE_TEX);
	// Load spritesheet 
	ResourceManager::LoadTexture("textures/Tiles/tiles_spritesheet.png", GL_FALSE, ResourceManager::TILES_SPRITESHEET);
	// Load world
	m_world.Load("WorldMaps/World1.map", this->Width, this->Height);

	// Set render-specific controls
	Shader shader = ResourceManager::GetShader(ResourceManager::SPRITE_SHADER);
	Renderer = new SpriteRenderer(shader);
}

void Game::ProcessInput(GLfloat dt)
{
}

void Game::Update(GLfloat dt)
{
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		// Draw background
		//Texture2D bgTex = ResourceManager::GetTexture(ResourceManager::BACKGROUND_TEX);
		//Renderer->DrawSprite(bgTex,	glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		// Draw level
		m_world.Draw(*Renderer);
	}
}
