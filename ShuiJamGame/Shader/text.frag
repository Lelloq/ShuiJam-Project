#version 450 core
out vec4 FragColour;

in vec2 uv;

uniform sampler2D text;
uniform float transparency = 1;

void main()
{
	FragColour = texture(text, uv) * transparency;
	if(FragColour.a == 0.0) discard;
}