#include "EBO.h"
#include <glad/glad.h>

namespace SJ
{
	unsigned EBO::currentlyBoundID = 0;
	//Generate indices with desired draw type
	EBO::EBO(const void* data, uint32_t count, unsigned int drawtype) : m_count(count)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, drawtype);
	}

	//Deletes the current buffer
	EBO::~EBO()
	{
		EBO::currentlyBoundID = 0;
		glDeleteBuffers(1, &m_ID); 
	}

	//Binds the index buffer
	void EBO::Bind() const
	{
		if(EBO::currentlyBoundID != m_ID)
		{
			EBO::currentlyBoundID = m_ID;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		}
	}

	//Unbinds index buffer
	void EBO::Unbind() const
	{
		EBO::currentlyBoundID = 0;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
