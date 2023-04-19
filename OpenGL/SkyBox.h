#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <vector>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Camera.h"

class SkyBox {
public:
	SkyBox(const std::string& path);

	void Draw(Camera& camer);
	inline unsigned int getTexID() { return this->texID; }

private:
	unsigned int texID;
	Shader* shader;
	VertexArray* vao;
	VertexBuffer* vbo;
	VertexBufferLayout* layout;
	std::vector<std::string> faces;
	std::vector<float> skyboxVertices;
	void LoadTexture();
};