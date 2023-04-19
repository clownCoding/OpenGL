#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Random.h"

struct ParticleProps {
	glm::vec3 position;
	glm::vec3 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime = 1.0f;
	glm::vec3 F;
};

class ParticleSystem {
public:
	ParticleSystem();

	void OnUpdate(float time);
	void OnRender(Camera& camera);

	void Emit(const ParticleProps& particleProps, float time);

private:
	struct Particle {
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 a;
		glm::vec4 colorBegin, colorEnd;
		float rotation = 0.0f;
		float sizeBeign, sizeEnd;

		float lifeTime = 1.0f;
		float lifeRemaining = 0.0f;

		float m = 0.1f;

		bool active = false;
	};
	std::vector<Particle> particlePool;
	unsigned int poolIndex = 9999;

	Shader* particleShader;
	VertexArray* VAO;
	VertexBuffer* VBO;
	VertexBufferLayout* layout;
	IndexBuffer* EBO;
};
