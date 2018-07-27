#include "Game.h"
#include "ResourceManager.h"

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
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(ResourceManager::SPRITE_SHADER).Use().SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("Textures/awesomeface.png", GL_TRUE, ResourceManager::FACE_TEX);
	// Set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader(ResourceManager::SPRITE_SHADER));
}

void Game::ProcessInput(GLfloat dt)
{
}

void Game::Update(GLfloat dt)
{
}

void Game::Render()
{
	Renderer->DrawSprite(ResourceManager::GetTexture(ResourceManager::FACE_TEX), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
