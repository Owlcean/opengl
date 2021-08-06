#include "Shader.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "Render.h"

Shader::Shader(const std::string& filePath):m_FilePath(filePath),m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filePath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind()const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind()const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float arg0, float arg1, float arg2, float arg3)
{
	GLCall(glUniform4f(GetUniformLocation(name), arg0, arg1, arg2, arg3)); 
}

void Shader::SetUniform3f(const std::string& name, float arg0, float arg1, float arg2)
{
	GLCall(glUniform3f(GetUniformLocation(name), arg0, arg1, arg2)); 
}

void Shader::SetUniformMatrix4f(const std::string& name, glm::mat4 matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	GLCall(unsigned int location = glGetUniformLocation(m_RendererID,name.c_str()));
	m_UniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1,
	};

	std::ifstream stream(filePath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	std::cout << stream._Stdstr << std::endl;
	while (getline(stream, line))
	{
		std::cout << line << std::endl;
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };

}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "fail to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl;
		std::cout << message << std::endl;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string&vertexShader, const std::string &fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}
