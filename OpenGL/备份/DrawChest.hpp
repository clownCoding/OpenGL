#pragma once
#include "glm.hpp"
#include "Shader.h"
#include "gtc/matrix_transform.hpp"
#include "Camera.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"



glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

float position[] = {
	//positon				//texture
	//正面
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,
	//后面
   -0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
};

unsigned int indices[] = {
	//正面
	0, 1, 2,
	0, 2, 3,
	//反面
	4, 5, 6,
	4, 6, 7,
	//左面
	4, 0, 3,
	4, 3, 7,
	//右面
	1, 5, 6,
	1, 6, 2,
	//上面
	3, 2, 6,
	3, 6, 7,
	//下面
	0, 1, 5,
	0, 5, 4
};

float vertices[] = {
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	  0.0f,  0.0f, -1.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	  0.0f,  0.0f, -1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	  0.0f,  0.0f, -1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	  0.0f,  0.0f, -1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	  0.0f,  0.0f, -1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	  0.0f,  0.0f, -1.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	  0.0f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	  0.0f,  0.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	  0.0f,  0.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	  0.0f,  0.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	  0.0f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	  0.0f,  0.0f, 1.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,

 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	  1.0f,  0.0f,  0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	  1.0f,  0.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	  1.0f,  0.0f,  0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,	  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	  0.0f, -1.0f,  0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	  0.0f, -1.0f,  0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	  0.0f, -1.0f,  0.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	  0.0f,  1.0f,  0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,	  0.0f,  1.0f,  0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	  0.0f,  1.0f,  0.0f
};

void drawChest(Camera camera) {

	VertexArray cubeVa;
	VertexBuffer cubeVb(vertices, sizeof(vertices));
	VertexBufferLayout layout;

	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	cubeVa.AddBuffer(cubeVb, layout);

	Shader shader("res/shader/Basic.shader");

	Texture diffuseTex("res/texture/container2.png");
	Texture specularTex("res/texture/container2_specular.png");

	VertexArray lightVAO;
	lightVAO.AddBuffer(cubeVb, layout);
	Shader lightShader("res/shader/Vertex.shader", "res/shader/Fragment.shader");

	Renderer render;

	glm::mat4 view = camera.GetView();
	glm::mat4 proj;
	proj = glm::perspective(glm::radians(camera.GetZoom()), (float)800 / (float)600, 0.1f, 100.0f);

	for (int i = 0; i < 4; i++) {
		glm::mat4 lightModel(1.0f);
		lightModel = glm::translate(lightModel, pointLightPositions[i]);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		lightShader.Bind();
		lightShader.SetUniformMat4f("u_MVP", proj * view * lightModel);

		render.DrawArray(lightVAO, lightShader);
	}

	shader.Bind();

	glm::mat4 model(1.0);
	shader.SetUniformMat4f("model", model);
	shader.SetUniformMat4f("view", view);
	shader.SetUniformMat4f("projection", proj);

	//shader.SetUniform3f("objColor", 0.976f, 0.8f, 0.886f);
	shader.SetUniform1i("material.diffuse", 0);
	shader.SetUniform1i("material.specular", 1);
	shader.SetUniform1f("material.shininess", 32.0f);

	diffuseTex.Bind();
	specularTex.Bind(1);

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);
	shader.SetUniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader.SetUniform3f("dirLight.ambient", 0.2f, 0.2f, 0.2f);
	shader.SetUniform3f("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
	shader.SetUniform3f("dirLight.specular", 0.8f, 0.8f, 0.8f);

	for (int i = 0; i < 4; i++) {
		std::string name = "pointLights[";
		name += 48 + i;
		name += "]";

		std::string position = name + ".position";
		std::string ambient = name + ".ambient";
		std::string diffuse = name + ".diffuse";
		std::string specular = name + ".specular";
		std::string constantK = name + ".constantK";
		std::string linearK = name + ".linearK";
		std::string quadraticK = name + ".quadraticK";

		shader.SetUniformVec3(position, pointLightPositions[i]);
		shader.SetUniformVec3(ambient, ambientColor);
		shader.SetUniformVec3(diffuse, diffuseColor);
		shader.SetUniformVec3(specular, lightColor);
		shader.SetUniform1f(constantK, 1.0f);
		shader.SetUniform1f(linearK, 0.09f);
		shader.SetUniform1f(quadraticK, 0.032f);
	}

	//shader.SetUniformVec3("spotLight.position", camera.GetPos());
	//shader.SetUniformVec3("spotLight.direction", camera.GetFront());
	//shader.SetUniform3f("spotLight.ambient", 0.2f, 0.2f, 0.2f);
	//shader.SetUniform3f("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
	//shader.SetUniform3f("spotLight.specular", 0.9f, 0.9f, 0.9f);
	//shader.SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	//shader.SetUniform1f("spotLight.outCutOff", glm::cos(glm::radians(17.5f)));

	//shader.SetUniform3f("viewPos", camera.GetPos().x, camera.GetPos().y, camera.GetPos().z);

	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shader.SetUniformMat4f("model", model);

		render.DrawArray(cubeVa, shader);
	}

}