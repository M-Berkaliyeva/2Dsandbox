#ifndef BACKGROUND_H
#define BACKGROUND_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "SpriteBatchRenderer.h"
#include "Texture.h"

/*******************************************************************
Background of the game with parallax layers 
each with different speed
********************************************************************/

class Background
{
public: 
	enum LayerName
	{
		SKY_LAYER = 0,
		//STARS_LAYER,
		//SUN_MOON_LAYER,
		MOUNTAINS_LAYER,
		HILLS_LAYER,
		//TREES_LAYER,
		CLOUDS_LAYER,
		OVERLAY_LAYER
	};

	struct Layer
	{
		GLint texID;
		GLuint shaderID;
		GLfloat depth;
	};

public:
	static const int NUMBER_OF_LAYERS = 5;
	static const GLfloat LAYERS_SPEEDS[NUMBER_OF_LAYERS];

	Background();
	~Background();

	void Update(GLfloat deltaTime, GLfloat gameTime);
	void Draw(SpriteBatchRenderer &renderer);
	void OnLoad(GLfloat width, GLfloat height);

private:
	void Animate(GLfloat deltaTime);

	Layer	m_layers[NUMBER_OF_LAYERS]; // holds all layers

	GLfloat m_layerUVu[NUMBER_OF_LAYERS]; // holds u parameter of layer UV
	GLfloat m_bgWidth;
	GLfloat m_bgHeight;
};
#endif //BACKGROUND_H