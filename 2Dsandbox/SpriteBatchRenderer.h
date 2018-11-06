#ifndef SPRITE_BATCH_RENDERER_H
#define SPRITE_BATCH_RENDERER_H
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
	TEXTURE,
	SHADER
};

class RenderBatch
{
public:
	RenderBatch(GLuint offs, GLuint numV, GLuint tex, GLuint shaderID) 
		: offset(offs), numVertices(numV), texture(tex), shader(shaderID)	{}
	GLuint offset;
	GLuint numVertices;
	GLuint texture;
	GLuint shader;
};

class SpriteBatchRenderer
{

public:
	SpriteBatchRenderer();
	~SpriteBatchRenderer();

	void init();
	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
	void end();

	void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture,  GLuint shader, float depth, const Color &color);

	void renderBatch();
	void createRenderBatches();
private:
	void createVertexArray();
	void sortGlyphs();

	//functions to do comparisons for sorting
	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);
	static bool compareShader(Glyph* a, Glyph* b);

	GLuint m_VBO;
	GLuint m_VAO;

	GlyphSortType m_sortType;
	std::vector<Glyph*> m_glyphPointers; // for sorting
	std::vector<Glyph> m_glyphs;
	std::vector<RenderBatch> m_renderBatches;
};
#endif