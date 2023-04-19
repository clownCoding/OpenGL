#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource {
	std::string vertexShader;
	std::string fragmentShader;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filePath);
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();
	inline unsigned int getID() { return m_RendererID; }

	void SetUniform4f(const std::string& name, glm::vec4 vec);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform1i(const std::string& name, int v0);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat);
	void SetUniformVec3(const std::string& name, const glm::vec3& vec);
	
	void Bind() const;
	void Unbind() const;

private:
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseSource(std::string filePath);
	unsigned int CompileShader(unsigned int type, std::string& source);
	unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader);
	std::string GetSingleSource(const std::string& filePath);
};
