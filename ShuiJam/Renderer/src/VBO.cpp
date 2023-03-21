/*\file VBO.cpp*/
#include "VBO.h"
#include <glad/glad.h>

namespace SJ
{
	//Creates the vbo with the changeable draw type
	//Need to be done since at some point dynamic geometry will be drawn
	VBO::VBO(const void* data, uint32_t size, unsigned int drawtype)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, drawtype);
	}

	//Deletes vertex buffers
	VBO::~VBO()
	{
		glDeleteBuffers(1, &m_ID);
	}

	//Binds vertex buffer
	void VBO::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void VBO::Edit(uint32_t size, const void* data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	//Unbinds vertex buffer
	void VBO::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
