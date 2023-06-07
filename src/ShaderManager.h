#pragma once

#include "Common.h"
#include "Shader.h"

#include <unordered_map>

struct Camera;

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void Update(const Camera &camera) const;

	void AddShader(const char *name, Shader* shader);

	Shader *GetShader(const char *name) { return m_shaderMap[name]; }

private:
	std::unordered_map<const char*, Shader*> m_shaderMap;

};
