/*\file Shader.h*/
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace SJ
{
	/*\class Shader*/
	class Shader
	{
	private:
		uint32_t m_ID; //!< Shader ID
	public:
		Shader(const char* vertexPath, const char* fragPath); //!< Compile the shaders
		void use();//!< Use the shader

		//!Uniform setters
		void setFloat(const std::string &name, float val);//!< Set float uniform
		void setInt(const std::string &name, int val);//!< Set int uniform
		void setBool(const std::string &name, bool val);//!< Set bool uniform
	};
}