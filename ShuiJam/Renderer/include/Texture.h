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
		Texture(const char* filepath); //!< Load texture from file path
		~Texture();//!< Deconstructor that deletes texture
		void Bind(uint32_t unit);//!< Bind texture to a texture unit
	};
}