#pragma once

#include "Common.h"
#include "Math.h"

class Shader
{
public:
	Shader();
	Shader(const char *vertexPath, const char *fragPath);
	~Shader();

	void Create(const char* vertexPath, const char* fragPath);

	void Use();

	void SetBoolean(const std::string &name, const bool &value) const;
	void SetInteger(const std::string &name, const int &value) const;
	void SetFloat(const std::string &name, const float &value) const;
	void SetMatrix4(const std::string &name, const glm::mat4 &value) const;
	void SetVector4(const std::string &name, const glm::vec4 &value) const;
	void SetVector3(const std::string &name, const glm::vec3 &value) const;
	void SetVector2(const std::string &name, const glm::vec2 &value) const;

	const inline unsigned int &GetShaderID() { return m_shaderID; }

private:
	unsigned int m_shaderID;

};
