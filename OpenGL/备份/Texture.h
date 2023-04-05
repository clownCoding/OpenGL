#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>

#include "stb_image.h"

class Texture {
private:
	unsigned m_RendererID;
	int width;
	int height;
	int BPP;
	std::string filePath;
	unsigned char* imageBuffer;
	std::string type;
public:
	Texture(const std::string& filePath, const std::string& type);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
	inline std::string getPath() { return filePath; }
	inline std::string getType() { return type; }
};
