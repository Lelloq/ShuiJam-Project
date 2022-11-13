/*\file Texture.cpp*/
#include "Texture.h"

namespace SJ
{
	Texture::Texture(const char* filepath)
	{
		//Generate and bind texture to texture unit 0
		glGenTextures(1, &m_ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		//Set texture parameters wrapping and filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Don't need to generate mipmaps since the game is in 2D

		//If data exists generate the texture
		int width, height, channels;
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		//Free image data from memory
		stbi_image_free(data);
	}

	//Delete texture
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}

	//Bind the texture to chosen texture unit
	void Texture::Bind(uint32_t unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}