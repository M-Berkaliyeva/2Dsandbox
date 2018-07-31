#version 330 core
in vec2 vert;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

vec2 texels;

void main()
{
	color = vec4(spriteColor, 1.0) * texture(image, vert);
}