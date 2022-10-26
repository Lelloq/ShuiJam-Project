/*\file BufferLayout.h*/
#pragma once
#include "Renderer.h"
#include <iostream>
#include <vector>

//Buffer elements struct - used by the VAO to create the vertex layout
struct BufferElements
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	//Made static so that the function can be accessed directly without having to create an instance of BufferElements
	static unsigned int GetSizeofType(unsigned int type)
	{
		switch(type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		}
#ifdef DEBUG
		std::cout << "Invalid type" << std::endl;
#endif
		return 0;
	}
};

//Buffer layout class - uses the buffer element class and pushes the layout strides and type
class BufferLayout
{
private:
	std::vector<BufferElements> m_elements;
	unsigned int m_stride;
public:
	BufferLayout() : m_stride(0){};

	//Create generic type so that other types can be put into the template arguments
	template<typename T>
	void Push(unsigned int count){}

	//Pushes float type into the elements and increases stride by 4
	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT,count,GL_FALSE });
		m_stride += BufferElements::GetSizeofType(GL_FLOAT) * count;
	}

	//Pushes float type into the elements and increases stride by 4
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
		m_stride += BufferElements::GetSizeofType(GL_UNSIGNED_INT) * count;
	}

	//Getters for elements and strides
	inline std::vector<BufferElements> GetElements() const { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }
};