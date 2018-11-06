#version 330 core

in vec2 fragmentUV;
in vec4 fragmentColor;

out vec4 color;

vec3 colorA = vec3(0.149,0.141,0.912);
vec3 colorB = vec3(1.000,0.833,0.224);

uniform sampler2D image;
uniform float time;

void main()
{
	float pct = abs(sin(time));
    // Mix uses pct (a value from 0-1) to
    // mix the two colors
	vec3 colMix = mix(colorA, colorB, pct);
	//fragmentColor = ;
	
	color = vec4(colMix, 1.0) * texture(image, fragmentUV);
}