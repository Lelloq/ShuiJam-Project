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
#include "Texture.h"
#include "VAO.h"
#include "EBO.h"

namespace SJ
{

	class Text
	{
	private:
		VAO* m_VAO;
		VBO* m_VBO;
		EBO* m_EBO;
		FT_Library m_library;
		FT_Face m_fontFace;
	public:
		Text();
		~Text();

	};
}
