/*\file Renderer.cpp*/
#include "Renderer.h"

void Renderer::Draw(VAO& vao, EBO& ebo, Shader& shader) const
{
	shader.use();
	vao.Bind();
	glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_BYTE, 0);
}
