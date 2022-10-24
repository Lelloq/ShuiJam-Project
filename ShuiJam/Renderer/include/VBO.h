#pragma once
#include "Renderer.h"

class VBO
{
private:
	GLuint m_ID;
public:
	VBO(const void* data, unsigned int size, GLenum drawtype);
	~VBO();

	void Bind() const;
	void Unbind() const;
};