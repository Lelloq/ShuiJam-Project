/*\file Texture.cpp*/
#include "Texture.h"
#include <stb_image.h>
#include <glad/glad.h>

namespace SJ
{
	Texture::Texture(std::string& filepath, int wrapping)
	{
		//Generate and bind texture to texture unit 0
		glGenTextures(1, &m_ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		if (filepath.find(".png") != std::string::npos)
		{
			stbi_set_flip_vertically_on_load(true);
		}
		else
		{
			stbi_set_flip_vertically_on_load(false);
		}

		//If data exists generate the texture
		int width, height, channels;
		unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		if (data)
		{
			//Choose the right format based on the data gathered above
			GLenum format;
			switch (channels)
			{
			case 1: format = GL_RED; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
			default: format = GL_RGB;
			}

			//Set active texture to 0 and bind the texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ID);
			//Create the texture image with the parameters
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			//Generate mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);

			//Texture parameter settings
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		}
		//Free data from memory so no memory leaks
		stbi_image_free(data);
	}

	//Delete texture
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}
}