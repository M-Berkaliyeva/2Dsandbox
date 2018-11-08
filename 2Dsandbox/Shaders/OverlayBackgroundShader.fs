#version 330 core

in vec2 fragmentUV;
in vec4 fragmentColor;

out vec4 color;

vec3 nightColorTop = vec3(0.0,0.0,0.0);
vec3 nightColorBot = vec3(0.302,0.408,0.471);

uniform sampler2D image;
uniform float time;

void main()
{
	float pct = abs(1 - cos(time));
   
   // Calculate gradients
	//vec3 nightColGradient = nightColorTop * (1 - fragmentUV.y) + nightColorBot * fragmentUV.y;
	// slowly fade out with time
	float alpha = smoothstep(0.6, 0.0, pct);

	color = vec4(nightColorTop, alpha);
}