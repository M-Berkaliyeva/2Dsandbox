#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


class SpriteBatchRenderer
{
	struct Glyph {
		GLuint TextureID;
		float depth;

		glm::vec2 topLeft;
		glm::vec2 bottomLeft;
		glm::vec2 topRight;
		glm::vec2 bottomRight;
	};

public:
	SpriteBatchRenderer();
	~SpriteBatchRenderer();

	void init();
	void begin();
	void end();

	void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const glm::vec4 &color);

	void rednerBatch();

private:
	GLuint m_VBO;
	GLuint m_VAO;

	std::vector<Glyph> m_glyphs;
};