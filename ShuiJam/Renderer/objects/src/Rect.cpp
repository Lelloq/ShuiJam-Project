#include "objects/include/Rect.h"
#include "Renderer.h"

namespace SJ
{
	Rect::Rect(const glm::vec2& pos,const glm::vec2& size,const int zIndex, Texture& image)
	{
		m_position = pos;
		m_size = size;

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

		m_texture = new Texture(image);//Reset is used to indicate that its now using the texture given by the constructor
		m_VAO = new VAO();//Create a new VAO
		m_VBO = new VBO(static_cast<void*>(m_verts.data()), sizeof(m_verts), GL_STATIC_DRAW);//Create a VBO with params for the constructor
		m_EBO = new EBO(static_cast<void*>(m_indices.data()), m_indices.size(), GL_STATIC_DRAW);//Create an EBO with params for the constructor

		BufferLayout layout;
		layout.Push<float>(3);//Positions
		layout.Push<float>(2);//UV coords

		m_VAO->AddBuffer(*m_VBO, layout);
	}

	Rect::~Rect()
	{
		m_texture->~Texture();
		m_VAO->~VAO();
		m_VBO->~VBO();
		m_EBO->~EBO();
	}

	void Rect::Draw(Shader& shader)
	{
		shader.use();
		m_VAO->Bind();
		m_EBO->Bind();
		uint32_t unit;
		//if(Renderer::textureUnitManager.full()) Renderer::textureUnitManager.clear();
		if(Renderer::textureUnitManager.getUnit(m_texture->getID(), unit))
		{
			m_texture->bind(unit);
		}
		glDrawElements(GL_TRIANGLES, m_EBO->GetCount(), GL_UNSIGNED_INT, 0);
		shader.setInt("image", unit);
	}
}