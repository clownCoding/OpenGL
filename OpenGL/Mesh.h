#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "CheckError.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	std::string type;
	std::string path;
	unsigned int id;
};

class Mesh {
private:
	VertexArray* VAO;
	VertexBuffer* VBO;
	VertexBufferLayout* layout;
	IndexBuffer* EBO;
	
	void setupMesh();

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader shader);
};
