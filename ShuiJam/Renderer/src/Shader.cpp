/*\file Shader.h*/
#include "Shader.h"
#include <glad/glad.h>


namespace SJ
{
	Shader::Shader(std::string& vertexPath, std::string& fragPath)
	{
		//Setup of shader code and shader file stream
		std::string vertexCode;
		std::string fragCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//Exceptions to throw if shader files fails
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//Open the shader files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragPath);
			std::stringstream vStream, fStream;
			//Put in the shader file buffers into the string streams
			vStream << vShaderFile.rdbuf();
			fStream << fShaderFile.rdbuf();
			//Close the shader files
			vShaderFile.close();
			fShaderFile.close();
			//Put streams into a string
			vertexCode = vStream.str();
			fragCode = fStream.str();
		}
		catch(std::ifstream::failure e)
		{
			std::cout << "ERROR SHADER FILE FAIL TO READ" << std::endl;
		}

		unsigned int vertex, fragment;
		int succ;
		char info[512];
		const char* vCode = vertexCode.c_str();
		const char* fCode = fragCode.c_str();

		//Create vertex shaders
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &succ);
		if(!succ)
		{
			glGetShaderInfoLog(vertex, 512, NULL, info);
			std::cout << "ERROR VERTEX SHADER FAILED COMPILATION" << std::endl;
		}

		//Create fragment shaders
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &succ);
		if (!succ)
		{
			glGetShaderInfoLog(fragment, 512, NULL, info);
			std::cout << "ERROR FRAGMENT SHADER FAILED COMPILATION" << std::endl;
		}
		//Create shader program
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertex);
		glAttachShader(m_ID, fragment);
		glLinkProgram(m_ID);
		checkCompileErrors(m_ID, "PROGRAM");
		glGetProgramiv(m_ID, GL_LINK_STATUS, &succ);
		if (!succ)
		{
			glGetProgramInfoLog(m_ID, 512, NULL, info);
			std::cout << "SHADER LINKING FAILED" << std::endl;
		}
		//Delete shader program to free memory
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::use()
	{
		glUseProgram(m_ID);
	}

	void Shader::setFloat(const std::string& name, float val)
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()),val);
	}

	void Shader::setInt(const std::string& name, int val)
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()),val);
	}

	void Shader::setBool(const std::string& name, bool val)
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()),val);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::checkCompileErrors(unsigned int shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
}
