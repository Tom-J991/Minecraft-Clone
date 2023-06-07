#include "TextureManager.h"

#include <glad/glad.h>

TextureManager::TextureManager()
{
	spdlog::info("Created Texture Manager.");
}
TextureManager::~TextureManager()
{
	for (auto &[name, texture] : m_textureMap)
	{
		if (texture)
			delete texture;
		texture = nullptr;
		spdlog::info("Removed Texture.");
	}
	m_textureMap.clear();

	spdlog::info("Destroyed Texture Manager.");
}

void TextureManager::AddTexture(const char *name, Texture *texture)
{
	spdlog::info("Added Texture [{0}]", name);
	m_textureMap.insert(std::pair<const char*, Texture*>(name, texture));
}
