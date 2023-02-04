#include "VAO.h"
#include <glad/glad.h>

namespace SJ
{
	//Generate 1 vertex array assigned to the id
	VAO::VAO()
	{
		glGenVertexArrays(1, &m_ID);
	}

	//Delete the created vertex array
	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	//Adds the layout to the vertex buffer
	void VAO::AddBuffer(VBO& vb, BufferLayout& layout)
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

	//Binds vertex array
	void VAO::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	//Unbinds vertex array
	void VAO::Unbind() const
	{
		glBindVertexArray(0);
	}
}
