/*\file Shader.h*/
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

namespace SJ
{
	/*\class Shader*/
	class Shader
	{
	private:
		uint32_t m_ID; //!< Shader ID
		static unsigned shaderInUse;
	public:
		Shader(std::string& vertexPath, std::string& fragPath); //!< Compile the shaders
		void use();//!< Use the shader

		//!Uniform setters
		void setFloat(const std::string &name, float val);//!< Set float uniform
		void setInt(const std::string &name, int val);//!< Set int uniform
		void setBool(const std::string &name, bool val);//!< Set bool uniform
		void setMat4(const std::string& name, const glm::mat4& mat);
		void setVec3(const std::string& name, const glm::vec3& vec);
		void checkCompileErrors(unsigned int shader, std::string type);
	};
}