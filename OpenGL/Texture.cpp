#include "Texture.h"

Texture::Texture(const std::string& filePath):m_RendererID(0), imageBuffer(nullptr), width(0), height(0), BPP(0), filePath(filePath)
{
	//stbi_set_flip_vertically_on_load(1);
	imageBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4);
		
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (imageBuffer)
		stbi_image_free(imageBuffer);
	else
		std::cout << "Faile to load texture!" << std::endl;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
