#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

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
	float u;
	float v;
};

struct Vertex
{
	//Vertex positions
	Position position;
	//Vertex colors;
	Color color;
	//Vertex UV coordinates
	UV uv;

	//*************Setters to make things easier****************
	void setPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void setUV(float u, float v)
	{
		uv.u = u;
		uv.v = v;
	}
	//*************************************************************
};

class Glyph {
public:
	Glyph() {};
	Glyph(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const Color & color) :
		textureID(texture), depth(depth)
	{
		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	};
	GLuint textureID;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
};
