#include "pch.h"
#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &m_ID);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VAO::AddBuffer(const VBO& vb, const BufferLayout& layout)
{
	Bind();
	vb.Bind();
	const std::vector<BufferElements>& elements = layout.GetElements();
	int offset = 0;
	for(int i = 0; i < elements.size(); i++)
	{
		const BufferElements e = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, e.count, e.type, e.normalized, layout.GetStride(), (const void*)offset);
		offset += e.count * BufferElements::GetSizeofType(e.type);
	}
}

void VAO::Bind() const
{
	glBindVertexArray(m_ID);
}

void VAO::Unbind() const
{
	glBindVertexArray(0);
}
