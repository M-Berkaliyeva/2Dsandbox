#version 330 core

in vec2 fragmentUV;
in vec4 fragmentColor;

out vec4 color;

uniform sampler2D image;

void main()
{
	color = fragmentColor * texture(image, fragmentUV);
}