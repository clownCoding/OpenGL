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
	int BoneIDs[MAX_BONE_INFLUENCE] = { 0 };
	float BoneWeights[MAX_BONE_INFLUENCE] = { 0.0f };
	void AddBoneData(int id, float weight) {
		for (unsigned int i = 0; i < MAX_BONE_INFLUENCE; i++) {
			if (BoneWeights[i] == 0.0f) {
				BoneIDs[i] = id;
				BoneWeights[i] = weight;
				return;
			}
		}
		assert(0);
	}
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
