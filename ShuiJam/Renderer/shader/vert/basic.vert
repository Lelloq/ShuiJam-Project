#version 450 core

layout (location = 0) in vec3 aPos;//Vec3 since I want to use the z axis to display specific elements infront of others
layout (location = 1) in vec2 aUV;//UV coordinates

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;

void main()
{
	gl_Position = projection * view * model * vec4(aPos,1.0);
	uv = aUV;
}