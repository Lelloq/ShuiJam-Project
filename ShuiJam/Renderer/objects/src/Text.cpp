/*****************************************************************//**
 * \file   Text.cpp
 * \date   March 2023
 *********************************************************************/
#include "objects/include/Text.h"
#include <glad/glad.h>
#include "Utils/Properties.h"
#include "Renderer.h"

namespace SJ
{
	void Text::InitFT(std::string fontFile)
	{
		if (FT_Init_FreeType(&m_ft)) { std::cout << "Failed to init freetype library" << "\n"; }
		if (FT_New_Face(m_ft, (SJFOLDER + FONTS + fontFile).c_str(), 0, &m_face)) { std::cout << "Failed to load font face" << "\n"; }
	}

	Text::Text(const glm::vec2& pos, std::wstring text, unsigned int width, unsigned int fontsize, unsigned int zIndex, std::string fontFile)
	{
		InitFT(fontFile);

		m_text = text;
		m_size = fontsize;
		m_pos = pos;
		m_width = width;

		float x = 1.0f;
		if(fontsize * text.size() > width)
		{
			x = fontsize * text.size() / width;
		}

		//Reversed the positions so that the text renders the right way up
		m_verts =
		//POSITION												//UV_COORDS
		{pos.x, pos.y + fontsize * 2,							static_cast<float>(zIndex), 0.0f, 0.0f,
		 pos.x + fontsize * text.size(), pos.y + fontsize * 2,	static_cast<float>(zIndex), x, 0.0f,
		 pos.x + fontsize * text.size(), pos.y,					static_cast<float>(zIndex), x, 1.0f,
		 pos.x, pos.y,											static_cast<float>(zIndex), 0.0f, 1.0f, };


		if(FT_Set_Pixel_Sizes(m_face, 0, fontsize)) { std::cout << "Failed to set font size." << "\n"; }
		FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);

		//Get bitmap width so it can be used for centre alignment later
		int bitmapWidth = 0;
		for (int i = 0; i < m_text.size(); i++)
		{
			if (FT_Load_Char(m_face, m_text.at(i), FT_LOAD_RENDER)) std::cout << "Failed to load character " << m_text.at(i) << "\n";
			else
			{
				bitmapWidth += m_face->glyph->bitmap.width;
			}
		}
		m_bitwidth = bitmapWidth;

		//Create an empty texture
		m_texture = std::make_unique<Texture>(fontsize * text.size(), fontsize * 2, 1, nullptr);

		m_VAO = std::make_unique<VAO>();
		m_VBO = std::make_unique<VBO>(static_cast<void*>(m_verts.data()), sizeof(m_verts), GL_DYNAMIC_DRAW);
		m_EBO = std::make_unique<EBO>(static_cast<void*>(m_indices.data()), m_indices.size(), GL_STATIC_DRAW);

		BufferLayout layout;
		layout.Push<float>(3);//Positions
		layout.Push<float>(2);//UV coords

		m_VAO->AddBuffer(*m_VBO, layout);
	}

	void Text::Draw(Shader& shader)
	{
		uint32_t unit;
		if (Renderer::textureUnitManager.full()) Renderer::textureUnitManager.clear();
		if (Renderer::textureUnitManager.getUnit(m_texture->getID(), unit))
		{
			m_texture->bind(unit);
		}

		//Bind buffers and shader
		shader.use();
		m_VAO->Bind();
		m_VBO->Bind();
		m_EBO->Bind();

		bool needsUpdating = false;
		if (m_firstEdit) 
		{ 
			m_firstEdit = false;
			needsUpdating = true;
		}

		if(m_isTextDifferent)
		{
			int bitmapWidth = 0;
			for (int i = 0; i < m_text.size(); i++)
			{
				if (FT_Load_Char(m_face, m_text.at(i), FT_LOAD_RENDER)) std::cout << "Failed to load character " << m_text.at(i) << "\n";
				else
				{
					bitmapWidth += m_face->glyph->bitmap.width;
				}
			}

			//VBO needed to be edited in order to prevent stretching of the text, squishing is intentional
			float x = 1.0f;
			if (bitmapWidth > m_width)
			{
				x = bitmapWidth / m_width;
			}

			m_verts =
				//POSITION												//UV_COORDS
			{m_pos.x,  m_pos.y + m_size * 2,							m_verts[2], 0.0f, 0.0f,
			 m_pos.x + m_size * m_text.size(), m_pos.y + m_size * 2,	m_verts[2], x, 0.0f,
			 m_pos.x + m_size * m_text.size(), m_pos.y,					m_verts[2], x, 1.0f,
			 m_pos.x,  m_pos.y,											m_verts[2], 0.0f, 1.0f, };
			m_VBO->Edit(sizeof(m_verts), m_verts.data());
			
			//Resize texture and reallocate space for the new text
			m_isTextDifferent = false;
			m_texture->resize(m_size * m_text.size(), m_size * 2, 1);
			needsUpdating = true;
			m_bitwidth = bitmapWidth;
		}

		/*
		APPROACH
		Edit the texture using offsets and size for each character
		Render the text at the end of the for loop
		*/
		if(needsUpdating)
		{
			unsigned xOffset = 0;
			glClearTexImage(m_texture->getID(), 0, GL_RED, GL_UNSIGNED_BYTE, 0);
			for (int i = 0; i < m_text.size(); i++)
			{
				if (FT_Load_Char(m_face, m_text.at(i), FT_LOAD_RENDER)) std::cout << "Failed to load character " << m_text.at(i) << "\n";
				else
				{
					unsigned width = m_face->glyph->bitmap.width;
					unsigned height = m_face->glyph->bitmap.rows;
					unsigned bearing = m_face->glyph->bitmap_top;
					float advance = m_face->glyph->advance.x >> 6;
					m_texture->edit(xOffset, 1.5 * m_size - bearing, width, height, m_face->glyph->bitmap.buffer);
					xOffset += advance;
				}
			}
		}

		shader.setInt("image", unit);
		glDrawElements(GL_TRIANGLES, m_EBO->GetCount(), GL_UNSIGNED_INT, 0);
	}

	void Text::changeText(std::wstring text)
	{
		m_text = text;
		m_isTextDifferent = true;
	}

	void Text::repositionVerts(glm::vec2 pos)
	{
		float x = 1.0f;
		if (m_size * m_text.size() > m_width)
		{
			x = m_size * m_text.size() / m_width;
		}

		m_verts =
		//POSITION													//UV_COORDS
		{pos.x,  pos.y + m_size * 2,							m_verts[2], 0.0f, 0.0f,
		 pos.x + m_size * m_text.size(), pos.y + m_size * 2,	m_verts[2], x, 0.0f,
		 pos.x + m_size * m_text.size(), pos.y,					m_verts[2], x, 1.0f,
		 pos.x,  pos.y,											m_verts[2], 0.0f, 1.0f, };
		m_pos = pos;
		m_VBO->Edit(sizeof(m_verts), m_verts.data());
	}
}
