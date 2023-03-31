/*\file Texture.cpp*/
#include "Texture.h"
#include "Renderer.h"
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
			m_channels = channels;
			m_width = width;
			m_height = height;
			//Choose the right format based on the data gathered above
			GLenum format;
			switch (channels)
			{
			case 1: format = GL_RED; glPixelStorei(GL_UNPACK_ALIGNMENT, 1); break;
			case 3: format = GL_RGB; glPixelStorei(GL_UNPACK_ALIGNMENT, 4); break;
			case 4: format = GL_RGBA; glPixelStorei(GL_UNPACK_ALIGNMENT, 4); break;
			default: format = GL_RGB; glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			}
			Renderer::textureUnitManager.clear();
			//Set active texture to 0 and bind the texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ID);
			//Create the texture image with the parameters
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

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
	Texture::Texture(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data)
	{
		m_channels = channels;
		m_width = width;
		m_height = height;

		glGenTextures(1, &m_ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		Renderer::textureUnitManager.clear();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 1) 
		{ 
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data); 
		}
		else if (channels == 3) 
		{ 
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
		}
		else if (channels == 4) 
		{ 
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); 
		}
		else return;
	}
	void Texture::bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
	void Texture::reloadTexture(std::string& filepath, int wrapping)
	{
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
			m_channels = channels;
			m_width = width;
			m_height = height;
			//Choose the right format based on the data gathered above
			GLenum format;
			switch (channels)
			{
			case 1: format = GL_RED; glPixelStorei(GL_UNPACK_ALIGNMENT, 1); break;
			case 3: format = GL_RGB; glPixelStorei(GL_UNPACK_ALIGNMENT, 4); break;
			case 4: format = GL_RGBA; glPixelStorei(GL_UNPACK_ALIGNMENT, 4); break;
			default: format = GL_RGB; glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			}

			//Create the texture image with the parameters
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			//Texture parameter settings
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		//Free data from memory so no memory leaks
		stbi_image_free(data);
	}
	void Texture::resize(uint32_t width, uint32_t height, uint32_t channels)
	{
		m_width = width;
		m_height = height;
		m_channels = channels;

		if (channels == 1) 
		{ 
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL); 
		}
		if (channels == 3)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		}
		else if (channels == 4) 
		{ 
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); 
		}
	}
	void Texture::edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data)
	{
		//Change the pixel store alignment depending on the channel format
		if (data)
		{
			if (m_channels == 1)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTextureSubImage2D(m_ID, 0, xOffset, yOffset, width, height, GL_RED, GL_UNSIGNED_BYTE, data);
			}
			if (m_channels == 3)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				glTextureSubImage2D(m_ID, 0, xOffset, yOffset, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else if (m_channels == 4) 
			{ 
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				glTextureSubImage2D(m_ID, 0, xOffset, yOffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data); 
			}
		}
	}
}