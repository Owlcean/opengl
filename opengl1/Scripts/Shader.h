#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	unsigned int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string &source);
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CreateShader(const std::string&vertexShader, const std::string &fragmentShader);
	std::unordered_map < std::string, unsigned int > m_UniformLocationCache;
public:
	Shader(const std::string &filePath);
	~Shader();

	void Bind()const;
	void Unbind()const;
	bool CompileShader();
	void SetUniform4f(const std::string& name, float arg0, float arg1, float arg2, float arg3);

};