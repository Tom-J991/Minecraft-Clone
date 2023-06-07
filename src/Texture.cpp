#include "Texture.h"

#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

Texture::~Texture()
{ 
	spdlog::info("Destroying Texture.");
	glDeleteTextures(1, &m_textureID);
}

void Texture::Create(const char *imagePath)
{
	spdlog::info("Creating Texture.");

	std::string imageFullPath = std::filesystem::absolute(imagePath).string();

	int width, height, nrChannels;

	spdlog::info("Loading Texture Image @[{0}]", imageFullPath.c_str());
	//stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

	m_textureWidth = width;
	m_textureHeight = height;
	
	spdlog::info("Generating Texture.");
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		spdlog::error("Failed to load texture @[{0}]", imageFullPath.c_str());
	}
	stbi_image_free(data);
}

void Texture::Use()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}
