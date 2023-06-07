#pragma once

#include "Common.h"

class Texture
{
public:
	Texture() { }
	Texture(const char *imagePath) { Create(imagePath); }
	~Texture();

	void Create(const char *imagePath);
	void Use();

	const inline int GetWidth() { return m_textureWidth; }
	const inline int GetHeight() { return m_textureHeight; }

	const inline unsigned int &GetTextureID() { return m_textureID; }

private:
	unsigned int m_textureID;
	int m_textureWidth, m_textureHeight;

};
