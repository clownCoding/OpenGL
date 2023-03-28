#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include<GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClaerError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
	void DrawElement(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	void DrawArray(const VertexArray& va, const Shader& shader);
	void Clear();
};
