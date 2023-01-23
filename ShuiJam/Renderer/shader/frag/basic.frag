#version 450 core

layout(location = 0) out vec4 Colour;

in vec2 uv;
in flat int texUnit;

uniform sampler2D[32] images;

void main()
{
	Colour = texture(images[texUnit], uv);
}