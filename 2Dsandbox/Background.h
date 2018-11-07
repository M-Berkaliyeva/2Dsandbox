#ifndef BACKGROUND_H
#define BACKGROUND_H

#define GLEW_STATIC
#include <GL/glew.h>

#include "SpriteBatchRenderer.h"
#include "Texture.h"

/*******************************************************************
Background of the game with parallax layers 
each with different speed
********************************************************************/

class Background
{
public:
	static const int NUMBER_OF_LAYERS = 5;
	static const float LAYERS_SPEEDS[NUMBER_OF_LAYERS];

	void Update(GLfloat deltaTime);
	void Draw(SpriteBatchRenderer &renderer);
	void OnLoad();

private:
	void Animate(GLfloat deltaTime);

	Texture2D	m_layerTextures[NUMBER_OF_LAYERS]; // holds all parallax layers
};
#endif //BACKGROUND_H