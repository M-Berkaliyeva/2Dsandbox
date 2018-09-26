#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->m_shader = shader;
	//this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->m_VAO);
}

void SpriteRenderer::createSprite(glm::vec4 color, glm::vec4 uvRect)
{
	float vertices[] = {
		// positions          // colors								// texture coords
		0.0f,	1.0f,		color.x, color.y, color.z, color.w,		uvRect.x, uvRect.y + uvRect.w, // top left
		0.0f,	0.0f,		color.x, color.y, color.z, color.w,		uvRect.x, uvRect.y, // bottom left
		1.0f,	0.0f,		color.x, color.y, color.z, color.w,		uvRect.x + uvRect.z, uvRect.y, // bottom right 

		1.0f,	0.0f,		color.x, color.y, color.z, color.w,		uvRect.x + uvRect.z, uvRect.y, // bottom right 
		1.0f,	1.0f,		color.x, color.y, color.z, color.w,		uvRect.x + uvRect.z, uvRect.y + uvRect.w, // top right
		0.0f,	1.0f,		color.x, color.y, color.z, color.w,		uvRect.x, uvRect.y + uvRect.w  // top left
	};

	unsigned int VBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D & texture, glm::vec2 position,
	glm::vec2 size, GLfloat rotate)
{
	// Prepare transformations
	this->m_shader.Use();
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

	this->m_shader.SetMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	// Render textured quad
	glBindVertexArray(this->m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
	// Configure VAO/VBO
	// Creating vertices for a quad with (0, 0) being top-left corner (good for 2D games)
	//***********************************************************************************
	// TODO: here we have single texture per quad, but what I eventually would want is to have
	// a tilemap from where i can extract needed texture, so we will have one texture and 
	// change only texCoords
	// TODO:read about texture arrays
	//***********************************************************************************
	
	/*GLfloat vertices[] = {
		//Pos			//Tex
		/*0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f,
		0.0f, 0.0f,		0.0f, 0.0f,

		0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f
		0.0f, 0.0f,		
		0.0f, 1.0f,		
		1.0f, 1.0f,		

		1.0f, 1.0f,		
		1.0f, 0.0f,		
		0.0f, 0.0f	
	};

	GLfloat Colors[] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	GLfloat UVs[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	glGenVertexArrays(1, &this->m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//TODO: read more about this static draw, maybe will need to change for my game

	glBindVertexArray(this->m_VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0); //(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);//
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);*/

	/*float vertices[] = {
		// positions          // colors					// texture coords
		0.0f,	1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, // top left
		0.0f,	0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f, // bottom left
		1.0f,	0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, // bottom right 

		1.0f,	0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, // bottom right 
		1.0f,	1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f, // top right
		0.0f,	1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // top left
	};

	unsigned int VBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/
}
