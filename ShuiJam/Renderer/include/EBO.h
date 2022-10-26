/*\file EBO.h*/
#pragma once
#include "Renderer.h"

class EBO
{
private:
	GLuint m_ID;
	unsigned int m_count;
public:
	EBO(const void* data, unsigned int count, GLenum drawtype);
	~EBO();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_count; }
};