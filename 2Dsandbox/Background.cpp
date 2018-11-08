#include "Background.h"

const GLfloat Background::LAYERS_SPEEDS[Background::NUMBER_OF_LAYERS] = { 0.0f,		// Sky - gradien (no texture)
																		//0.0f,		// Stars (sparkling)
																		//0.0f,		// Sun / Moon (rotating)
																		0.0001f,	// Mountains (relative to player speed)
																		0.0f,		// Hills (relative to player speed)
																		//0.0f,		// Trees (relative to player speed)
																		0.0005f,	// Clouds (constant speed)
																		0.0f };		// Overlay (at  night make everything darker)

Background::Background()
{

}

Background::~Background()
{

}

void Background::OnLoad(GLfloat width, GLfloat height)
{	
	m_bgWidth = width;
	m_bgHeight = height;

	m_layers[SKY_LAYER].texID = -1; // NO TEXTURE
	m_layers[SKY_LAYER].shaderID = ResourceManager::GetShader(ResourceManager::SKY_BACKGROUND_SHADER)->ID;
	m_layers[SKY_LAYER].depth = 15.0f;

	m_layers[MOUNTAINS_LAYER].texID = ResourceManager::GetTexture(ResourceManager::CLOUDS_BG_BACKGROUND_TEXTURE)->texture.ID;
	m_layers[MOUNTAINS_LAYER].shaderID = ResourceManager::GetShader(ResourceManager::TEXTURED_SPRITE_SHADER)->ID;
	m_layers[MOUNTAINS_LAYER].depth = 14.0f;

	m_layers[HILLS_LAYER].texID = ResourceManager::GetTexture(ResourceManager::MOUNTAINS_BACKGROUND_TEXTURE)->texture.ID;
	m_layers[HILLS_LAYER].shaderID = ResourceManager::GetShader(ResourceManager::TEXTURED_SPRITE_SHADER)->ID;
	m_layers[HILLS_LAYER].depth = 13.0f;

	m_layers[CLOUDS_LAYER].texID = ResourceManager::GetTexture(ResourceManager::CLOUDS_FG_BACKGROUND_TEXTURE)->texture.ID;
	m_layers[CLOUDS_LAYER].shaderID = ResourceManager::GetShader(ResourceManager::TEXTURED_SPRITE_SHADER)->ID;
	m_layers[CLOUDS_LAYER].depth = 12.0f;

	m_layers[OVERLAY_LAYER].texID = -1; // NO TEXTURE
	m_layers[OVERLAY_LAYER].shaderID = ResourceManager::GetShader(ResourceManager::OVERLAY_BACKGROUND_SHADER)->ID;
	m_layers[OVERLAY_LAYER].depth = -12.0f; // IN FRONT OF EVERYTHING

	for (int i = 0; i < NUMBER_OF_LAYERS; i++)
		m_layerUVu[i] = 0.0f;
}

void Background::Update(GLfloat deltaTime, GLfloat gameTime)
{
	for (int i = 0; i < NUMBER_OF_LAYERS; i++)
		m_layerUVu[i] += LAYERS_SPEEDS[i];

	ResourceManager::GetShader(ResourceManager::SKY_BACKGROUND_SHADER)->SetFloat("time", gameTime, true);
	ResourceManager::GetShader(ResourceManager::OVERLAY_BACKGROUND_SHADER)->SetFloat("time", gameTime, true);
}

void Background::Draw(SpriteBatchRenderer & renderer)
{
	for (int i = 0; i < NUMBER_OF_LAYERS; i++)
	{
		// Add background layers to batch
		glm::vec4 pos(0.0f, 0.0f, m_bgWidth, m_bgHeight);
		glm::vec4 uv(m_layerUVu[i], 0.0f, 1.0f, 1.0f);
		Color col; col.r = 255; col.g = 255; col.b = 255; col.a = 255;
		renderer.draw(pos, uv, col, m_layers[i].shaderID, m_layers[i].texID, m_layers[i].depth);
	}	
}

void Background::Animate(GLfloat deltaTime)
{
}
