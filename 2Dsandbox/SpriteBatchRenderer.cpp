#include "SpriteBatchRenderer.h"

#include <algorithm>
#include <cstddef>

SpriteBatchRenderer::SpriteBatchRenderer() :
	m_VBO(0), m_VAO(0)
{

}

SpriteBatchRenderer::~SpriteBatchRenderer()
{

}

void SpriteBatchRenderer::init()
{
	createVertexArray();
}

void SpriteBatchRenderer::begin(GlyphSortType sortType)
{
	m_sortType = sortType;
}

void SpriteBatchRenderer::end()
{
	sortGlyphs();
}

void SpriteBatchRenderer::draw(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const Color & color)
{
	Glyph* newGlyph = new Glyph;
	newGlyph->TextureID = texture;
	newGlyph->depth = depth;

	newGlyph->topLeft.color = color;
	newGlyph->topLeft.setPosition(destRect.x, destRect.y);
	newGlyph->topLeft.setUV(uvRect.x, uvRect.y);

	newGlyph->bottomLeft.color = color;
	newGlyph->bottomLeft.setPosition(destRect.x, destRect.y + destRect.w);
	newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	newGlyph->topRight.color = color;
	newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y);
	newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	newGlyph->bottomRight.color = color;
	newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	m_glyphs.push_back(newGlyph);
}

void SpriteBatchRenderer::renderBatch()
{

}

void SpriteBatchRenderer::createVertexArray()
{
	if(m_VAO == 0)
	{
		glGenVertexArrays(1, &m_VAO);
	}
	glBindVertexArray(m_VAO);

	if (m_VBO == 0)
	{
		glGenBuffers(1, &m_VBO);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// This is a position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// This is a color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	// This is a uv attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// Unbind and disable
	glBindVertexArray(0);
}

void SpriteBatchRenderer::sortGlyphs()
{
	switch (m_sortType)
	{
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(m_glyphs.begin(), m_glyphs.end(), compareBackToFront);
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(m_glyphs.begin(), m_glyphs.end(), compareFrontToBack);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(m_glyphs.begin(), m_glyphs.end(), compareTexture);
		break;
	}
	
}

bool SpriteBatchRenderer::compareFrontToBack(Glyph * a, Glyph * b)
{
	return (a->depth < b->depth);//sort by depth
}

bool SpriteBatchRenderer::compareBackToFront(Glyph * a, Glyph * b)
{
	return (a->depth > b->depth);//sort depth
}

bool SpriteBatchRenderer::compareTexture(Glyph * a, Glyph * b)
{
	return (a->TextureID < b->TextureID);//sort by texture ID
}
