/*\file Shader.h*/
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

/*\class Shader*/
class Shader
{
private:
	GLuint m_ID; //!< Shader ID
public:
	Shader(const char* vertexPath, const char* fragPath); //!< Compile the shaders
	void use();//!< Use the shader

	//Uniform setters
	void setFloat(const std::string &name, float val);
	void setInt(const std::string &name, int val);
	void setBool(const std::string &name, bool val);
};