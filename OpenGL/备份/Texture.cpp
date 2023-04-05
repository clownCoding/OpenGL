#include "Texture.h"

Texture::Texture(const std::string& filePath, const std::string& type):m_RendererID(0), imageBuffer(nullptr), width(0), height(0), BPP(0), filePath(filePath)
{
	//stbi_set_flip_vertically_on_load(1);
	this->type = type;
	
	//glGenTextures(1, &m_RendererID);
	//imageBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 0);
	//
	//if (imageBuffer) {
	//	GLenum format = GL_RGBA;
	//	if (BPP == 1)
	//		format = GL_RED;
	//	else if (BPP == 3)
	//		format = GL_RGB;
	//	else if (BPP == 4)
	//		format = GL_RGBA;

	//	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageBuffer);
	//	glGenerateMipmap(GL_TEXTURE_2D);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	stbi_image_free(imageBuffer);
	//}
	//else {
	//	std::cout << "Faile to load texture!" << std::endl;
	//	stbi_image_free(imageBuffer);
	//}

	glGenTextures(1, &m_RendererID);

    imageBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 0);
	if (imageBuffer)
	{
		GLenum format;
		if (BPP == 1)
			format = GL_RED;
		else if (BPP == 3)
			format = GL_RGB;
		else if (BPP == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(imageBuffer);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filePath << std::endl;
		stbi_image_free(imageBuffer);
	}

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
