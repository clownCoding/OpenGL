#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>

void printMat(glm::mat4 matrix) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void printMat(aiMatrix4x4 matrix) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
glm::mat4 aiMatToGlmMat(const aiMatrix4x4* from) {
	glm::mat4 to(1.0f);
	to[0][0] = (float)from->a1; to[0][1] = (float)from->b1;  to[0][2] = (float)from->c1; to[0][3] = (float)from->d1;
	to[1][0] = (float)from->a2; to[1][1] = (float)from->b2;  to[1][2] = (float)from->c2; to[1][3] = (float)from->d2;
	to[2][0] = (float)from->a3; to[2][1] = (float)from->b3;  to[2][2] = (float)from->c3; to[2][3] = (float)from->d3;
	to[3][0] = (float)from->a4; to[3][1] = (float)from->b4;  to[3][2] = (float)from->c4; to[3][3] = (float)from->d4;
	return to;
}

void Model::Draw(Shader& shader, Shader* noTexShader)
{
	for (unsigned int i = 0; i < boneInfos.size(); i++) {
		std::string name = "boneTransform.gBones[";
		name += std::to_string(i);
		name += "]";
		glm::mat4 transformation(aiMatToGlmMat(&(boneInfos[i].transformation)));
		shader.SetUniformMat4f(name, transformation);
	}
	for (int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader, noTexShader);
}

void Model::loadModel(std::string const& path, std::string const& animPath) 
{
	scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	animation = new BoneAnimation(animPath);
	directory = path.substr(0, path.find_last_of('/'));
	rootInverseTransform = scene->mRootNode->mTransformation.Inverse();
	processNode(scene->mRootNode, scene);
	std::cout << boneInfos.size() << std::endl;
}

int Model::GetBoneId(const aiBone* bone) {
	int boneIndex = 0;
	std::string name = bone->mName.C_Str();

	if (name_id.find(name) == name_id.end()) {
		boneIndex = name_id.size();
		name_id[name] = boneIndex;
	}
	else {
		boneIndex = name_id[name];
	}

	return boneIndex;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureInfo> textures;
	scene->mMaterials[0];
	//process vertex
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		vertex.Position = position;

		if (mesh->HasNormals())
		{
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertex.Normal = normal;
		}
		
		if (mesh->mTextureCoords[0]) {
			glm::vec2 tex;
			tex.x = mesh->mTextureCoords[0][i].x;
			tex.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = tex;
		}

		aiColor3D aiColor;
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);

		glm::vec3 color;
		color.x = aiColor.r;
		color.y = aiColor.g;
		color.z = aiColor.b;
		vertex.Color = color;
		vertices.push_back(vertex);
	}
	//process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	//process texture
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<TextureInfo> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<TextureInfo> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	}
	//process bones
	if (mesh->HasBones()) {
		for (unsigned int i = 0; i < mesh->mNumBones; i++) {
			aiBone* bone = mesh->mBones[i];
			int boneId = GetBoneId(bone);

			if (boneId == boneInfos.size()) {
				BoneInfo info(bone->mOffsetMatrix);
				boneInfos.push_back(info);
			}
	
			for (unsigned int j = 0; j < bone->mNumWeights; j++) {
				const aiVertexWeight& vw = bone->mWeights[j];
				vertices[vw.mVertexId].AddBoneData(boneId, vw.mWeight);
			}
		}
	}
	
	return Mesh(vertices, indices, textures);
}

void Model::ProcessAnimation() {
	aiMatrix4x4 pTransform;
	double time = 0.0f;
	if (animation->HasAnimation()) {
		time = animation->GetAnimationTimeTicks();
	}
	ProcessTransform(time, scene->mRootNode, pTransform);
}

void Model::ProcessTransform(float time, const aiNode* node,const aiMatrix4x4& parentTransform)
{
	std::string nodeName(node->mName.data);
	aiMatrix4x4 animTransform;
	if (animation->HasAnimation()) {
		const aiNodeAnim* nodeAnim = animation->FindNodeAnim(nodeName);
		animTransform = animation->GetAimationTransform(time, nodeAnim);
		/*if (nodeAnim) {
			animTransform = animation->GetAimationTransform(time, nodeAnim);
		}
		else {
			animTransform = node->mTransformation;
		}*/
	}
	else {
		animTransform = node->mTransformation;
	}
	aiMatrix4x4 pTransform = parentTransform * animTransform;
	if (name_id.find(nodeName) != name_id.end()) {
		unsigned int boneIndex = name_id[nodeName];
		boneInfos[boneIndex].transformation = pTransform * boneInfos[boneIndex].offset;
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		ProcessTransform(time, node->mChildren[i], pTransform);
}

void Model::SetStartTime()
{
	animation->SetStartTime();
}

std::vector<TextureInfo> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureInfo>textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			TextureInfo texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.path = str.C_Str();
			texture.type = typeName;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + "/"  + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RGBA;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

