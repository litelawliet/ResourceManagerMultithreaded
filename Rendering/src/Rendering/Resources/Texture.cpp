#include<stdafx.h>

#include "Rendering/Resources/Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

Rendering::Resources::Texture::Texture()
	: m_loaded{false}
{
	glGenTextures(1, &m_id);

	Bind(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	Unbind();
}

Rendering::Resources::Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

void Rendering::Resources::Texture::Bind(const int p_slot) const
{
	glActiveTexture(GL_TEXTURE0 + p_slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Rendering::Resources::Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Rendering::Resources::Texture::Load(const std::string& p_filepath, const int p_slot)
{
	m_texturePath = p_filepath;
	Bind(p_slot);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::cout << "Texture loaded: " << m_texturePath.c_str() << "\n";
	int width;
	int height;
	int nrChannels;

	stbi_uc* data = stbi_load(m_texturePath.data(), &width, &height, &nrChannels, 0);
	stbi__vertical_flip(data, width, height, nrChannels);

	if (data != nullptr)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		m_loaded = true;
	}
	else
	{
		std::cout << "Failed to load texture at path : " << m_texturePath.c_str() << '\n';
		m_loaded = false;
	}

	stbi_image_free(data);
	Unbind();
}

inline const std::string& Rendering::Resources::Texture::GetTexturePath() const
{
	return m_texturePath;
}
