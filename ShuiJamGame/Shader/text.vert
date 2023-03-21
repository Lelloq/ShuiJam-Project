#version 450 core

layout (location = 0) in vec3 aPos;//Vec3 since I want to use the z axis to display specific elements infront of others
layout (location = 1) in vec2 aUV;//UV coordinates

out vec2 uv;

uniform mat4 model;
uniform mat4 projection;

layout (std140) uniform P
{
	mat4 p;
};

void main()
{
	gl_Position = projection * model * vec4(aPos,1.0);
	uv = aUV;
}