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
		virtual void Draw() const;//!< Virtual draw function for inherited classes
		void Draw(VAO& vao, EBO& vbo, Shader& shader) const;//!< Render stuff onto the screen without needing to create a class that draws it
		void Clear() const//!< Clears the window buffer and depth buffer bits
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	};
}