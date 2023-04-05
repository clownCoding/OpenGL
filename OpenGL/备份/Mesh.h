#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "CheckError.h"
#include "Texture.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	//glm::ivec4 m_BoneIDs[MAX_BONE_INFLUENCE];
	//glm::vec4 m_Weights[MAX_BONE_INFLUENCE];
};

struct TextureInfo {
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
	std::vector<TextureInfo> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures);
	void Draw(Shader shader);
};
