/*\file Renderer.cpp*/
#include "Renderer.h"
#include <glad/glad.h>

namespace SJ
{
	//Draw objects on screen
	void Renderer::Draw(VAO& vao, EBO& ebo, Shader& shader)
	{
		shader.use();
		vao.Bind();
		ebo.Bind();
		glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, 0);
	}
}
