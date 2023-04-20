#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	this->particlePool.resize(10000);
	this->particleShader = new Shader("res/shader/Particle.shader");

	float vertices[] = {
		//前面
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		//后面
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
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
	VAO = new VertexArray();
	VAO->Bind();
	VBO = new VertexBuffer(&vertices[0], sizeof(vertices));
	EBO = new IndexBuffer(&indices[0], (unsigned int)36);
	layout = new VertexBufferLayout();
	layout->Push<float>(3);
	VAO->AddBuffer(*VBO, *layout);
	EBO->Unbind();
	VBO->Unbind();
	VAO->Unbind();
}

void ParticleSystem::OnUpdate(float time)
{
	for (auto& particle : particlePool) {
		if (!particle.active) {
			continue;
		}

		if (particle.lifeRemaining <= 0.0f) {
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= time;
		particle.velocity += particle.a * time;
		particle.a = glm::vec3(0.0f, -8.0f, 0.0) + particle.velocity * -0.01f / particle.m;
		particle.position += particle.velocity * time;
		particle.rotation += 0.01f * time;
	}
}

void ParticleSystem::OnRender(Camera& camera)
{	
	for (auto& particle : particlePool) {
		if (!particle.active) {
			continue;
		}
		float life = particle.lifeRemaining / particle.lifeTime;
		glm::vec4 color = glm::lerp(particle.colorBegin, particle.colorEnd, life);
		float size = glm::lerp(particle.sizeBeign, particle.sizeEnd, life);

		glm::mat4 model(1.0f);
		model = glm::translate(model, particle.position);
		model = glm::rotate(model, particle.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(size * 0.2));
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)1920 / (float)1080, 0.1f, 100.0f);
		glm::mat4 view = camera.GetView();

		particleShader->Bind();
		particleShader->SetUniformMat4f("model", model);
		particleShader->SetUniformMat4f("view", view);
		particleShader->SetUniformMat4f("projection", projection);
		particleShader->SetUniform4f("u_Color", color);

		VAO->Bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
		VAO->Unbind();
		particleShader->Unbind();
	}
}

void ParticleSystem::Emit(const ParticleProps& particleProps, float time)
{
	Particle& particle = particlePool[poolIndex];
	particle.active = true;
	particle.position = particleProps.position;
	particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

	particle.velocity = particleProps.F * time / particle.m;
	particle.a = glm::vec3(0.0f, -8.0f, 0.0) + particle.velocity * -0.01f / particle.m;

	//color
	particle.colorBegin = particleProps.colorBegin;
	particle.colorEnd = particleProps.colorEnd;

	//lifeTime
	particle.lifeTime = particleProps.lifeTime;
	particle.lifeRemaining = particleProps.lifeTime;

	//size
	particle.sizeBeign = particleProps.sizeBegin;
	particle.sizeEnd = particleProps.sizeEnd;

	poolIndex = --poolIndex % particlePool.size();
}
