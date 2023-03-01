/*\file Renderer.cpp*/
#include "Renderer.h"
#include <glad/glad.h>

namespace SJ
{
	TextureUnitManager Renderer::textureUnitManager(16);//Giving it 16 slots since thats the minimum a gpu can have
	//Draw objects on screen
	void Renderer::Draw(VAO& vao, EBO& ebo, Shader& shader)
	{
		shader.use();
		vao.Bind();
		ebo.Bind();
		glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, 0);
	}
	void Renderer::DrawInstanced(VAO& vao, EBO& ebo, Shader& shader, unsigned int count)
	{
		shader.use();
		vao.Bind();
		ebo.Bind();
		glDrawElementsInstanced(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, 0, count);
	}
}
