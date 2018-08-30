#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D & texture, glm::vec2 position, 
	glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	// Prepare transformations
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0);
	// translate
	model = glm::translate(model, glm::vec3(position, 0.0f));

	// rotate - do we need this? 
	// move rotation origin to center, also take into account the scaling
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	// scale
	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.SetMatrix4("model", model);
	this->shader.SetVector3f("spriteColor", color);
	//this->shader.SetInteger("image", texID);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	// Render textured quad
	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}

void SpriteRenderer::initRenderData()
{
	// Configure VAO/VBO
	GLuint VBO;
	// Creating vertices for a quad with (0, 0) being top-left corner (good for 2D games)
	//***********************************************************************************
	// TODO: here we have single texture per quad, but what I eventually would want is to have
	// a tilemap from where i can extract needed texture, so we will have one texture and 
	// change only texCoords
	// TODO:read about texture arrays
	//***********************************************************************************
	
	GLfloat vertices[] = {
		//Pos			//Tex
		0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f,
		0.0f, 0.0f,		0.0f, 0.0f,

		0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//TODO: read more about this static draw, maybe will need to change for my game

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0); //(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);//
	
	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
