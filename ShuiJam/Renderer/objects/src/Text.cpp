#include "objects/include/Text.h"
#include <glad/glad.h>
#include "Utils/Properties.h"
#include "Renderer.h"

namespace SJ
{
	Text::Text(const glm::vec2& pos, std::wstring text, unsigned int size, unsigned int zIndex)
	{
		m_text = text;
		m_size = size;

		m_verts = //!<Default vertices
		//POSITION												 //UV_COORDS
		{pos.x, pos.y,			     static_cast<float>(zIndex), 0.0f, 0.0f,
		 pos.x + size, pos.y,		 static_cast<float>(zIndex), 1.0f, 0.0f,
		 pos.x + size, pos.y + size, static_cast<float>(zIndex), 1.0f, 1.0f,
		 pos.x, pos.y + size,		 static_cast<float>(zIndex), 0.0f, 1.0f, };

		if(FT_Init_FreeType(&m_ft)){ std::cout << "Failed to init freetype library" << "\n"; }
		if(FT_New_Face(m_ft, (SJFOLDER + FONTS + "NotoSerifJP-Regular.otf").c_str(), 0, &m_face)){ std::cout << "Failed to load font face" << "\n"; }
		if(FT_Set_Pixel_Sizes(m_face, 0, size)) { std::cout << "Failed to set font size." << "\n"; }

		FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);
		m_texture = new Texture(size * text.size(), size, 4, nullptr);

		m_VAO = new VAO();
		m_VBO = new VBO(static_cast<void*>(m_verts.data()), sizeof(m_verts), GL_DYNAMIC_DRAW);
		m_EBO = new EBO(static_cast<void*>(m_indices.data()), m_indices.size(), GL_STATIC_DRAW);

		BufferLayout layout;
		layout.Push<float>(3);//Positions
		layout.Push<float>(2);//UV coords

		m_VAO->AddBuffer(*m_VBO, layout);

		FT_Done_Face(m_face);
		FT_Done_FreeType(m_ft);
	}

	Text::~Text()
	{
		m_VAO->~VAO();
		m_VBO->~VBO();
		m_EBO->~EBO();
		m_texture->~Texture();
	}

	void Text::Draw(Shader& shader, std::wstring text)
	{
		bool needsUpdating = false;
		if(text != L"" || text != m_text)
		{
			m_text = text;
			m_texture->edit(0, 0, m_size * text.size(), m_size, nullptr);
			needsUpdating = true;
		}

		shader.use();
		m_VAO->Bind();
		m_VBO->Bind();
		m_EBO->Bind();

		if(needsUpdating)
		{
			for (auto c : m_text)
			{
				if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) std::cout << "Failed to load character " << c << "\n";
				else
				{
					unsigned width = m_face->glyph->bitmap.width;
					unsigned height = m_face->glyph->bitmap.rows;
					glm::vec2 bearing(m_face->glyph->bitmap_left, -m_face->glyph->bitmap_top);

					/*TODO
					Edit VBO so that vertices are positioned correctly on screen
					Find the character size and advance it

					APPROACH
					Edit the VBO once (size of a character in pixels * number of characters in x axis)
					Edit the texture using offsets and size for each character
					Render the text at the end of the for loop
					*/

					m_texture->edit(0, 0, 256, 256, m_face->glyph->bitmap.buffer);
				}
			}
		}

		uint32_t unit;
		if (Renderer::textureUnitManager.full()) Renderer::textureUnitManager.clear();
		if (Renderer::textureUnitManager.getUnit(m_texture->getID(), unit))
		{
			m_texture->bind(unit);
		}
		glDrawElements(GL_TRIANGLES, m_EBO->GetCount(), GL_UNSIGNED_INT, 0);
	}
}
