#include "ShaderManager.h"

#include "Camera.h"

ShaderManager::ShaderManager()
{
	spdlog::info("Created Shader Manager.");
}
ShaderManager::~ShaderManager()
{
	for (auto &[name, shader] : m_shaderMap)
	{
		if (shader)
			delete shader;
		shader = nullptr;
		spdlog::info("Removed Shader.");
	}
	m_shaderMap.clear();

	spdlog::info("Destroyed Shader Manager.");
}

void ShaderManager::Update(const Camera &camera) const
{
	for (auto &[name, shader] : m_shaderMap)
	{
		shader->Use();
		shader->SetMatrix4("view", camera.view);
		shader->SetMatrix4("proj", camera.proj);

		shader->SetVector3("cameraPos", camera.position);
	}
}

void ShaderManager::AddShader(const char *name, Shader *shader)
{
	spdlog::info("Added Shader [{0}]", name);
	m_shaderMap.insert(std::pair<const char*, Shader*>(name, shader));
}
