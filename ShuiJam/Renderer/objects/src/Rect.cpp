#include "objects/include/Rect.h"

namespace SJ
{
	Rect::Rect(const glm::vec2& pos,const glm::vec2& size,const int zIndex, Texture& image)
	{
		//The rectangle are drawn from the top left
		glm::vec2 posTL{pos.x, pos.y}; //Top left
		glm::vec2 posTR{pos.x + size.x, pos.y};//Top right
		glm::vec2 posBR{pos.x + size.x, pos.y + size.y };//Bottom right
		glm::vec2 posBL{pos.x, pos.y + size.y};//Bottom left

		m_verts =
		 //POSITIONS								   //UV_COORDS
		{posTL.x, posTL.y, static_cast<float>(zIndex), 0.0f, 0.0f,
		 posTR.x, posTR.y, static_cast<float>(zIndex), 1.0f, 0.0f,
		 posBR.x, posBR.y, static_cast<float>(zIndex), 1.0f, 1.0f,
		 posBL.x, posBL.y, static_cast<float>(zIndex), 0.0f, 1.0f, };

		m_texture.reset(&image);//Reset is used to indicate that its now using the texture given by the constructor
		m_VAO = std::make_unique<VAO>();//Create a new VAO
		m_VBO = std::make_unique<VBO>(m_verts, sizeof(m_verts), GL_STATIC_DRAW);//Create a VBO with params for the constructor
		m_EBO = std::make_unique<EBO>(m_indices, m_indices.size(), GL_STATIC_DRAW);//Create an EBO with params for the constructor

		BufferLayout layout;
		layout.Push<float>(3);//Positions
		layout.Push<float>(2);//UV coords

		m_VAO->AddBuffer(*m_VBO, layout);
	}

	Rect::~Rect()
	{
		m_VAO->~VAO();
		m_VBO->~VBO();
		m_EBO->~EBO();
		m_texture->~Texture();
	}
}