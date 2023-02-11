#version 450 core
out vec4 FragColour;

in vec2 uv;

uniform sampler2D image;

void main()
{
	FragColour = texture(image, uv);
}