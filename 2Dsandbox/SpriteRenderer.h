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
	void DrawSprite(Texture2D &texture, glm::vec2 position, 
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, 
		glm::vec3 color = glm::vec3(1.0f));

private:
	Shader shader;
	GLuint quadVAO;
	//Initializes and configures the quad's buffer and vertex attributes
	void initRenderData();
};

#endif