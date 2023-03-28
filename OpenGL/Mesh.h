#pragma once
#include <glm.hpp>
#include <vector>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "CheckError.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct TextureInfo {
	std::string type;
	std::string path;
	unsigned int id;
};

class Mesh {
private:
	//VertexArray* VAO;
	//VertexBuffer* VBO;
	//VertexBufferLayout* layout;
	//IndexBuffer* EBO;
	unsigned int VBO;
	unsigned int EBO;
	
	void setupMesh();

public:
	unsigned int VAO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureInfo> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures);
	void Draw(Shader shader);
};
