#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& filePath):m_FilePath(filePath)
{
	ShaderProgramSource source = ParseSource(filePath);
	this->m_RendererID = CreateShader(source.vertexShader, source.fragmentShader);
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	std::string vertexShader = GetSingleSource(vertexShaderPath);
	std::string fragmentShader = GetSingleSource(fragmentShaderPath);
	this->m_RendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(this->m_RendererID);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
	glUniform1f(GetUniformLocation(name), v0);
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
	glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniformMat4f(const std::string& name,const glm::mat4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3& vec)
{
	glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
}

void Shader::Bind() const{
	glUseProgram(this->m_RendererID);
}

void Shader::Unbind() const {
	glUseProgram(0);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1) {
		std::cout << "Warning: Uniform " << name << " doesn't exist" << std::endl;
	}
	m_UniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseSource(std::string filePath) {
	std::ifstream ifs(filePath);
	ShaderProgramSource source;
	if (!ifs.is_open()) {
		std::cout << "Faile to open shader file" << std::endl;
		return source;
	}
	std::string line;
	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGEMNT = 1
	};
	ShaderType type = ShaderType::NONE;
	std::stringstream ss[2];
	while (getline(ifs, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGEMNT;
			}
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}
	source = { ss[0].str(), ss[1].str() };
	return source;
}

unsigned int Shader::CompileShader(unsigned int type, std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//Error handing
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Faile to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(std::string& vertexShader, std::string& fragmentShader) {
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

std::string Shader::GetSingleSource(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	if (!ifs.is_open()) {
		std::cout << "faile to open shader file" << std::endl;
		return "";
	}
	std::string source;
	std::string line;
	while (getline(ifs, line)) {
		source += line + "\n";
	}
	return source;
}



