#pragma once
#include <glad/glad.h>
#include <iostream>

class FrameBuffer {
public:
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;

	FrameBuffer();
	~FrameBuffer();
	void Bind() const;
	void Unbind() const;
	void UseTex() const;

private:
	unsigned int id;
	unsigned int depthMap;

	void AttachDepth();
	void CheckError();
};
