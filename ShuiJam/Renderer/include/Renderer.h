/*\file Renderer.h*/
#pragma once
#include "Shader.h"
#include "VAO.h"
#include "EBO.h"

class Renderer
{
public:
	void Draw(VAO& vao, EBO& vbo, Shader& shader) const;//!< Render stuff onto the screen
	//Clears the window
	void Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
};