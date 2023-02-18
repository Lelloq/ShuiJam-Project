/*\file Renderer.h*/
#pragma once
#include "Shader.h"
#include "VAO.h"
#include "EBO.h"

namespace SJ
{
	class Renderer
	{
	public:
		static void Draw(VAO& vao, EBO& ebo, Shader& shader);//!< Render stuff onto the screen without needing to create a class that draws it
		static void DrawInstanced(VAO& vao, EBO& ebo, Shader& shader, unsigned int count);//!< Render stuff onto the screen without needing to create a class that draws it
	};
}