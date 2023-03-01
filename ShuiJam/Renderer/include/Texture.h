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
	public:
		Texture(std::string& filepath, int wrapping); //!< Load texture from file path
		~Texture();//!< Deconstructor that deletes texture
		void bind(unsigned int slot);
		[[nodiscard]] inline uint32_t getID() { return m_ID; }
	};
}