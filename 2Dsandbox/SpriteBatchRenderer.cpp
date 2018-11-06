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
	// Set up all pointers for fast sorting
	m_glyphPointers.resize(m_glyphs.size());
	for (int i = 0; i < m_glyphPointers.size(); i++)
	{
		m_glyphPointers[i] = &m_glyphs[i];
	}
	sortGlyphs();
	createRenderBatches();
}

void SpriteBatchRenderer::draw(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, GLuint shader, float depth, const Color & color)
{
	m_glyphs.emplace_back(destRect, uvRect, texture, shader,  depth, color);
}

void SpriteBatchRenderer::renderBatch()
{
	glBindVertexArray(m_VAO);
	for (int i = 0; i < m_renderBatches.size(); i++)
	{
		glUseProgram(m_renderBatches[i].shader);

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
	m_renderBatches.emplace_back(offset, 6, m_glyphPointers[0]->textureID, m_glyphPointers[0]->shaderID);
	vertices[currVertex++] = m_glyphPointers[0]->topLeft;
	vertices[currVertex++] = m_glyphPointers[0]->bottomLeft;
	vertices[currVertex++] = m_glyphPointers[0]->bottomRight;
	vertices[currVertex++] = m_glyphPointers[0]->bottomRight;
	vertices[currVertex++] = m_glyphPointers[0]->topRight;
	vertices[currVertex++] = m_glyphPointers[0]->topLeft;
	offset += 6;


	for (int currGlyph = 1; currGlyph < m_glyphPointers.size(); currGlyph++)
	{
		//only emplace render batch if curent texture is not the same as previous
		if(m_glyphPointers[currGlyph]->textureID != m_glyphPointers[currGlyph - 1]->textureID)
		{
			m_renderBatches.emplace_back(offset, 6, m_glyphPointers[currGlyph]->textureID, m_glyphPointers[currGlyph]->shaderID);
		}
		else//otherwise increase the size of current renderbatch
		{
			m_renderBatches.back().numVertices += 6;
		}
		
		vertices[currVertex++] = m_glyphPointers[currGlyph]->topLeft;
		vertices[currVertex++] = m_glyphPointers[currGlyph]->bottomLeft;
		vertices[currVertex++] = m_glyphPointers[currGlyph]->bottomRight;
		vertices[currVertex++] = m_glyphPointers[currGlyph]->bottomRight;
		vertices[currVertex++] = m_glyphPointers[currGlyph]->topRight;
		vertices[currVertex++] = m_glyphPointers[currGlyph]->topLeft;
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	// This is a color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	// This is a uv attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));

	// Unbind and disable
	glBindVertexArray(0);
}

void SpriteBatchRenderer::sortGlyphs()
{
	switch (m_sortType)
	{
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareBackToFront);
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareFrontToBack);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareTexture);
		break;
	case GlyphSortType::SHADER:
		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareShader);
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
	return (a->textureID < b->textureID);//sort by texture ID
}

bool SpriteBatchRenderer::compareShader(Glyph * a, Glyph * b)
{
	return (a->shaderID < b->shaderID);//sort by shader ID
}
