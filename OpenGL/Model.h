 #pragma once
#include "Mesh.h"
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include "BoneAnimation.h"

struct BoneInfo {
	aiMatrix4x4 offset;
	aiMatrix4x4 transformation;
	BoneInfo(const aiMatrix4x4& offset) {
		this->offset = offset;
	}
};

class Model {
public:
	inline Model(std::string path, std::string animPath, bool gamma = false):gammaCorrection(gamma), filePath(path) { loadModel(path, animPath); };
	inline Model(std::string path, bool gamma = false):gammaCorrection(gamma), filePath(path) { loadModel(path, path); };
	void Draw(Shader& shader, Shader* noTexShader = nullptr);
	void ProcessAnimation();
	void SetStartTime();

private:
	std::vector<TextureInfo> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	std::string filePath;
	bool gammaCorrection;
	std::vector<BoneInfo> boneInfos;
	std::map<std::string,unsigned int> name_id;
	BoneAnimation* animation;
	aiMatrix4x4 rootInverseTransform;
	Assimp::Importer import;
	const aiScene* scene;

	void loadModel(const std::string& path, const std::string& animPath);
	int GetBoneId(const aiBone* bone);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	void ProcessTransform(float time, const aiNode* node, const aiMatrix4x4& parentTransform);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};