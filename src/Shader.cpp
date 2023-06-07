#include "Shader.h"

#include <glad/glad.h>

#include <filesystem>
#include <fstream>

Shader::Shader()
{ }
Shader::Shader(const char *vertexPath, const char *fragPath)
{
	Create(vertexPath, fragPath);
}
Shader::~Shader()
{
	spdlog::info("Destroying Shader.");
	glDeleteProgram(m_shaderID);
}

void Shader::Create(const char *vertexPath, const char *fragPath)
{
	spdlog::info("Creating Shader.");

	std::string vertexFullPath = std::filesystem::absolute(vertexPath).string();
	std::string fragFullPath = std::filesystem::absolute(fragPath).string();

	std::string vertexCode, fragCode;
	std::ifstream vertexFile, fragFile;

	spdlog::info("Reading Shader Files.");
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		spdlog::info("Reading Vertex Shader @[{0}]", vertexFullPath.c_str());
		vertexFile.open(vertexPath);
		spdlog::info("Reading Fragment Shader @[{0}]", fragFullPath.c_str());
		fragFile.open(fragPath);
		std::stringstream vStream, fStream;
		
		vStream << vertexFile.rdbuf();
		fStream << fragFile.rdbuf();

		vertexFile.close();
		fragFile.close();

		vertexCode = vStream.str();
		fragCode = fStream.str();
	}
	catch (std::ifstream::failure e)
	{
		spdlog::error("Failed to read Shader Files!");
	}

	const char *vCode = vertexCode.c_str();
	const char *fCode = fragCode.c_str();

	unsigned int vertexShader, fragShader;
	int success;
	char infoLog[512];

	spdlog::info("Creating Vertex Shader.");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vCode, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		spdlog::error("Could not compile Vertex Shader @[{0}] : {1}", vertexFullPath.c_str(), infoLog);
	}

	spdlog::info("Creating Fragment Shader.");
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fCode, nullptr);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
		spdlog::error("Could not compile Fragment Shader @[{0}] : {1}", fragFullPath.c_str(), infoLog);
	}

	spdlog::info("Creating Shader Program.");
	m_shaderID = glCreateProgram();
	glAttachShader(m_shaderID, vertexShader);
	glAttachShader(m_shaderID, fragShader);
	glLinkProgram(m_shaderID);
	glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderID, 512, nullptr, infoLog);
		spdlog::error("Could not link shader program! : {0}", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void Shader::Use()
{
	glUseProgram(m_shaderID);
}

void Shader::SetBoolean(const std::string &name, const bool &value) const
{
	glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), (int)value);
}
void Shader::SetInteger(const std::string &name, const int &value) const
{
	glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), value);
}
void Shader::SetFloat(const std::string &name, const float &value) const
{
	glUniform1f(glGetUniformLocation(m_shaderID, name.c_str()), value);
}
void Shader::SetMatrix4(const std::string &name, const glm::mat4 &value) const
{
	unsigned int loc = glGetUniformLocation(m_shaderID, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::SetVector4(const std::string &name, const glm::vec4 &value) const
{
	unsigned int loc = glGetUniformLocation(m_shaderID, name.c_str());
	glUniform4fv(loc, 1, glm::value_ptr(value));
}
void Shader::SetVector3(const std::string &name, const glm::vec3 &value) const
{
	unsigned int loc = glGetUniformLocation(m_shaderID, name.c_str());
	glUniform3fv(loc, 1, glm::value_ptr(value));
}
void Shader::SetVector2(const std::string &name, const glm::vec2 &value) const
{
	unsigned int loc = glGetUniformLocation(m_shaderID, name.c_str());
	glUniform2fv(loc, 1, glm::value_ptr(value));
}
