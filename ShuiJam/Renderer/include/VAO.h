/*\file VAO.h*/
#pragma once
#include "Shader.h"
#include "VBO.h"
#include "BufferLayout.h"

namespace SJ
{
	/*\class VAO*/
	class VAO
	{
	private:
		uint32_t m_ID;//!< Renderer ID
	public:
		VAO();//!< Default constructor
		~VAO();//!< Destructor deletes vertex array

		void AddBuffer(VBO& vb, BufferLayout& layout);//!< Add layout to the buffer
		void Bind() const;//!< Bind buffer
		void Unbind() const;//!< Unbind buffer
		inline uint32_t getID() { return m_ID; }
	};
}
