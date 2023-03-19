/*\file Texture.h*/
#pragma once
#include <string>

namespace SJ
{
	/*\class Texture*/
	class Texture
	{
	private:
		uint32_t m_ID;//!< Texture ID
		int m_channels = 0;
	public:
		Texture(std::string& filepath, int wrapping); //!< Load texture from file path
		Texture(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data);//!< Load texture with raw data
		~Texture();//!< Deconstructor that deletes texture
		void reloadTexture(std::string& filepath, int wrapping);//!< Reload texture with a different image
		void edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data);//!< Edit existing texture
		void bind(unsigned int slot);
		[[nodiscard]] inline uint32_t getID() { return m_ID; }
	};
}