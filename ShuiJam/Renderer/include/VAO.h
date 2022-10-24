#pragma once
#include "Renderer.h"
#include "VBO.h"
#include "BufferLayout.h"

class VAO
{
private:
	GLuint m_ID;
public:
	VAO();
	~VAO();

	void AddBuffer(const VBO& vb, const BufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};
