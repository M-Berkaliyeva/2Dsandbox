#version 330 core

in vec2 fragmentUV;
in vec4 fragmentColor;

out vec4 color;

vec3 dayColorTop = vec3(0.059,0.357,0.380);
vec3 dayColorBot = vec3(0.494,0.683,0.636);
vec3 nightColorTop = vec3(0.051,0.067,0.082);
vec3 nightColorBot = vec3(0.302,0.408,0.471);

uniform float time;

void main()
{
	float pct = abs(1 - cos(time));
   
   // Calculate gradients
	vec3 dayColGradient = dayColorTop * (1 - fragmentUV.y) + dayColorBot * fragmentUV.y;
	vec3 nightColGradient = nightColorTop * (1 - fragmentUV.y) + nightColorBot * fragmentUV.y;

	// Mix uses pct (a value from 0-1) to
    // mix the two colors
	vec3 colMix = mix(dayColGradient, nightColGradient, pct);
	
	color = vec4(colMix, 1.0);// * texture(image, fragmentUV);
}