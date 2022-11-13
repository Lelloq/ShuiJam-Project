/*\file VBO.h*/
#pragma once
#include "Shader.h"

namespace SJ
{
	/*\class VBO*/
	class VBO
	{
	private:
		uint32_t m_ID;//!< Renderer ID
	public:
		VBO(const void* data, uint32_t size, GLenum drawtype);//!< Create buffer with specified data and size
		~VBO();//! Destructor deletes buffer

		void Bind() const;//!< Bind vbo
		void Unbind() const;//!< Unbind vbo
	};
}