#version 330 core
layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec2 vertexUV;

out vec2 fragmentUV;
out vec4 fragmentColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
void main()
{	
	fragmentUV = vertexUV;	
	fragmentColor = vertexColor;
	gl_Position = projection * view * model * vec4(vertexPosition, 0.0, 1.0);
}