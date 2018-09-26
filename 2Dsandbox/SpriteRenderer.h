#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Shader.h"
#include "Texture.h"
#include "ResourceManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SpriteRenderer
{
public:
	// Constructor (inits shaders/quad)
	SpriteRenderer(Shader &shader);
	// Destructor
	~SpriteRenderer();
	// Renders a defined quad textured with given sprite
	void createSprite(glm::vec4 color, glm::vec4 uvRect);
	void DrawSprite(Texture2D &texture, glm::vec2 position, 
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f
		);

private:
	Shader m_shader;
	GLuint m_VAO;
	//Initializes and configures the quad's buffer and vertex attributes
	void initRenderData();
};

#endif