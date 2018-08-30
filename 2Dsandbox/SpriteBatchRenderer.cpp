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
	m_renderBatches.clear();
	m_glyphs.clear();
}

void SpriteBatchRenderer::end()
{
	sortGlyphs();
	createRenderBatches();
}

void SpriteBatchRenderer::draw(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const Color & color)
{
	Glyph* newGlyph = new Glyph;
	newGlyph->TextureID = texture;
	newGlyph->depth = depth;

	newGlyph->topLeft.color = color;
	newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
	newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	newGlyph->bottomLeft.color = color;
	newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
	newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

	newGlyph->topRight.color = color;
	newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	newGlyph->bottomRight.color = color;
	newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
	newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	m_glyphs.push_back(newGlyph);
}

void SpriteBatchRenderer::renderBatch()
{
	glBindVertexArray(m_VAO);
	for (int i = 0; i < m_renderBatches.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);

		glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
	}
	glBindVertexArray(0);
}

void SpriteBatchRenderer::createRenderBatches()
{
	std::vector<Vertex> vertices;
	vertices.resize(m_glyphs.size() * 6);//allocate memory since we know size

	if (m_glyphs.empty())
	{
		return;
	}
	int offset = 0;
	int currVertex = 0;
	m_renderBatches.emplace_back(offset, 6, m_glyphs[0]->TextureID);
	vertices[currVertex++] = m_glyphs[0]->topLeft;
	vertices[currVertex++] = m_glyphs[0]->bottomLeft;
	vertices[currVertex++] = m_glyphs[0]->bottomRight;
	vertices[currVertex++] = m_glyphs[0]->bottomRight;
	vertices[currVertex++] = m_glyphs[0]->topRight;
	vertices[currVertex++] = m_glyphs[0]->topLeft;
	offset += 6;


	for (int currGlyph = 1; currGlyph < m_glyphs.size(); currGlyph++)
	{
		//only emplace render batch if curent texture is not the same as previous
		if(m_glyphs[currGlyph]->TextureID != m_glyphs[currGlyph - 1]->TextureID)
		{
			m_renderBatches.emplace_back(offset, 6, m_glyphs[currGlyph]->TextureID);
		}
		else//otherwise increase the size of current renderbatch
		{
			m_renderBatches.back().numVertices += 6;
		}
		
		vertices[currVertex++] = m_glyphs[currGlyph]->topLeft;
		vertices[currVertex++] = m_glyphs[currGlyph]->bottomLeft;
		vertices[currVertex++] = m_glyphs[currGlyph]->bottomRight;
		vertices[currVertex++] = m_glyphs[currGlyph]->bottomRight;
		vertices[currVertex++] = m_glyphs[currGlyph]->topRight;
		vertices[currVertex++] = m_glyphs[currGlyph]->topLeft;
		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//orphan the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW );
	//upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
