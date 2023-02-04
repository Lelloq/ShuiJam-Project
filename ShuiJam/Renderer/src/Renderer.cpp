/*\file Renderer.cpp*/
#include "Renderer.h"
#include <glad/glad.h>

namespace SJ
{
	//Draw objects on screen
	void Renderer::Draw(VAO& vao, EBO& ebo, Shader& shader) const
	{
		shader.use();
		vao.Bind();
		glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_BYTE, 0);
	}
	void Renderer::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
