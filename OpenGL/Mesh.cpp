#include "Mesh.h"

void Mesh::setupMesh()
{
	VAO = new VertexArray();
	VAO->Bind();
	VBO = new VertexBuffer(&vertices[0], sizeof(Vertex) * vertices.size());
	EBO = new IndexBuffer(&indices[0], (unsigned int)indices.size());

	layout = new VertexBufferLayout();
	layout->Push<float>(3);
	layout->Push<float>(3);
	layout->Push<float>(2);
	layout->Push<int>(4);
	layout->Push<float>(4);
	layout->Push<float>(3);
	VAO->AddBuffer(*VBO, *layout);
	EBO->Unbind();
	VBO->Unbind();
	VAO->Unbind();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	setupMesh();
}

void Mesh::Draw(Shader shader, Shader* noTexShader)
{
	if (textures.size() != 0) {
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			//std::string name = textures[i].getType();
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			shader.SetUniform1i((name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		VAO->Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		VAO->Unbind();
		glActiveTexture(GL_TEXTURE0);
	}
	else {
		noTexShader->Bind();
		VAO->Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		VAO->Unbind();
		noTexShader->Unbind();
	}
	
}
