/*****************************************************************//**
 * \file   Text.h
 * \brief  opengl text implementation that uses the ft2 library
 * 
 * \date   March 2023
 *********************************************************************/
#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include "Texture.h"
#include "VAO.h"
#include "EBO.h"

namespace SJ
{
	/*
	* \class Text
	* \brief the text class that uses opengl and freetype2
	*/
	class Text
	{
	private:
		std::array<float, 20> m_verts = //!<Default vertices
		//POSITION    //UV_COORDS
		{0.f, 0.f, 0, 0.0f, 0.0f,
		 1.f, 0.f, 0, 1.0f, 0.0f,
		 1.f, 1.f, 0, 1.0f, 1.0f,
		 0.f, 1.f, 0, 0.0f, 1.0f, };

		std::array<uint32_t, 6> m_indices = //!<Indices
		{
			0,1,3,
			3,2,1,
		};

		VAO* m_VAO;
		VBO* m_VBO;
		EBO* m_EBO;

		std::wstring m_text;
		unsigned int m_size;
		bool m_firstEdit = true;

		Texture* m_texture;
		FT_Library m_ft;
		FT_Face m_face;

		static bool FTBegan;
		void InitFT();
	public:
		Text(const glm::vec2& pos, std::wstring text, unsigned int size, unsigned int zIndex);
		~Text();
		void Draw(Shader& shader, std::wstring = L"");
	};
}
