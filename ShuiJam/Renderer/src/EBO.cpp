#include "EBO.h"

//Generate indices with desired draw type
EBO::EBO(const void* data, unsigned int count, GLenum drawtype) : m_count(count)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, drawtype);
}

//Deletes the current buffer
EBO::~EBO()
{
	glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_ID);
}

//Binds the index buffer
void EBO::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

//Unbinds index buffer
void EBO::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
