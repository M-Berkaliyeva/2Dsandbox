#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Vertex.h"

enum class GlyphSortType
{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

struct Glyph {
	GLuint TextureID;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
};

class SpriteBatchRenderer
{

public:
	SpriteBatchRenderer();
	~SpriteBatchRenderer();

	void init();
	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
	void end();

	void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const Color &color);

	void rednerBatch();

private:
	void createVertexArray();
	void sortGlyphs();

	GLuint m_VBO;
	GLuint m_VAO;

	GlyphSortType m_sortType;
	std::vector<Glyph*> m_glyphs;
};