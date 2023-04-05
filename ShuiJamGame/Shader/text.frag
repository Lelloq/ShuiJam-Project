#version 450 core
out vec4 FragColour;

in vec2 uv;

uniform sampler2D image;
uniform float transparency = 1;
uniform vec3 colour = vec3(1.0,1.0,1.0);

void main()
{
	FragColour = vec4(colour.xyz, texture(image, uv).r) * transparency;
	if(FragColour.a == 0.0) discard;
}