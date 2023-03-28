#pragma once
#include <string>
#include <GL/glew.h>
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
public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() { return width; };
	inline int GetHeight() { return height; };
};
