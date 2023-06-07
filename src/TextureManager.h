#pragma once

#include "Common.h"
#include "Texture.h"

#include <unordered_map>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void AddTexture(const char *name, Texture *texture);

	Texture *GetTexture(const char *name) { return m_textureMap[name]; }

private:
	std::unordered_map<const char*, Texture*> m_textureMap;

};
