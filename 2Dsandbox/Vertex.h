#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class Vertex
{
	struct Position
	{
		float x;
		float y;
	};

	struct Color
	{
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UV
	{
		float U;
		float V;
	};
};
