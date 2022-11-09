/*\file EBO.h*/
#pragma once
#include "Shader.h"

/*\class EBO*/
class EBO
{
private:
	uint32_t m_ID;//!< Renderer ID
	uint32_t m_count;//!< Indices count
public:
	EBO(const void* data, uint32_t count, GLenum drawtype);//!< Create EBO with specified data
	~EBO();//!< Delete ebo

	void Bind() const;//!< Bind EBO
	void Unbind() const;//!< Unbind EBO

	inline uint32_t GetCount() const { return m_count; }//!< Get indices count
};