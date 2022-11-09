/*\file Texture.h*/
#pragma once
#include <glad/glad.h>
#include <stb_image.h>
#include <string>

/*\class Texture*/
class Texture
{
private:
	unsigned int m_ID;//!< Texture ID
public:
	Texture(std::string filepath); //!< Load texture from file path
	~Texture();//!< Deconstructor that deletes texture
	void Bind(uint32_t unit);//!< Bind texture to a texture unit
};