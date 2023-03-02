#version 450 core
out vec4 FragColour;

in vec2 uv;

uniform sampler2D image;
uniform float transparency = 1;

void main()
{
	FragColour = texture(image, uv) * transparency;
}