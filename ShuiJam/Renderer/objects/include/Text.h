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

		std::unique_ptr<VAO> m_VAO;
		std::unique_ptr<VBO> m_VBO;
		std::unique_ptr<EBO> m_EBO;

		std::wstring m_text;
		unsigned int m_size;//This is the width of the entire text
		unsigned int m_width;//This one is the width of the quad that stores the text
		glm::vec2 m_pos;
		bool m_firstEdit = true;
		bool m_isTextDifferent = false;
		float m_bitwidth = 0;

		std::unique_ptr<Texture> m_texture;
		FT_Library m_ft;
		FT_Face m_face;

		/**
		 * \function InitFT(std::string fontFile).
		 * \brief Initialises freetype2 and loads the font file
		 * \param fontFile the font file
		 */
		void InitFT(std::string fontFile);
	public:
		/**
		 * Text constructor.
		 * \param pos - glm::vec2 the position of the text box on the bottom left origin
		 * \param text - std::wstring the text itself
		 * \param width - int width of the text box
		 * \param fontsize - int the fontsize
		 * \param zIndex - the z index, higher z number the more on top it will render above the other objects
		 * \param fontFile - the font file, can be .ttf .otf etc.
		 */
		Text(const glm::vec2& pos, std::wstring text,unsigned int width ,unsigned int fontsize, unsigned int zIndex, std::string fontFile);
		~Text();//! Destructor
		/**
		 * \function void repositionVerts(glm::vec2 pos).
		 * \brief Change the position of the object
		 * \param pos the x,y position
		 */
		void repositionVerts(glm::vec2 pos);
		/**
		 * \function void changeText(std::wstring text).
		 * \param text - std::wstring the new text
		 */
		void changeText(std::wstring text);
		/**
		 * \function void Draw(Shader& shader).
		 * \brief Draw the object to screen
		 * \param shader the shader class
		 */
		void Draw(Shader& shader);
		inline float getBitmapWidth() { return m_bitwidth; }//!<Get the width of the text based on the text width rather than the text box width
	};
}
