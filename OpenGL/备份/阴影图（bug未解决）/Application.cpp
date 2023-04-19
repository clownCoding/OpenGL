#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <irrKlang.h>
#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "CheckError.h"
#include "ParticleSystem.h"
#include "Random.h"
#include "FrameBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "SkyBox.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float time, ParticleSystem& ps);
void InitScene(Shader& shader, SkyBox skybox);
void InitRole(Shader& shader);
void EmitParticle(ParticleSystem& ps, float time);
void renderQuad();

int BoneNum = 0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 8.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera(cameraPos, cameraFront, cameraUp);

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();

int main(void) {
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	
	glfwSwapInterval(1);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	stbi_set_flip_vertically_on_load(false);

	Model scene("res/model/scene2/stage.pmx");
	Shader shader("res/shader/Basic.shader");
	Model ourModel("res/model/eula/eula.pmx");
		
	//ourModel.SetStartTime();
	//soundEngine->play2D("res/audio/music.mp3", GL_TRUE);

	ParticleSystem ps;

	SkyBox skybox("res/skybox");

	Shader simpleDepth("res/shader/SimpleDepth.shader");
	FrameBuffer depthFbo;
	Shader depthMapShader("res/shader/DepthMap.shader");
	depthMapShader.SetUniform1i("depthMap", 0);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.ProcessKeyboard(window, deltaTime);
		processInput(window, deltaTime, ps);
		ps.OnUpdate(deltaTime);
		ps.OnRender(camera);

		//loading depthMap
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		float near = 1.0f, far = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near, far);
		glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 5.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		simpleDepth.Bind();
		simpleDepth.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
		simpleDepth.SetUniformMat4f("model", model);
		glViewport(0, 0, 1024, 1024);
		/*depthFbo.Bind();*/
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		scene.Draw(simpleDepth);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		ourModel.ProcessAnimation();
		ourModel.Draw(simpleDepth);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//depthFbo.Unbind();

		//render scene
		//glEnable(GL_CULL_FACE);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//shader.Bind();
		//shader.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
		//glActiveTexture(GL_TEXTURE1);
		//depthFbo.UseTex();
		//InitScene(shader, skybox);
		//scene.Draw(shader);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		//InitRole(shader);
		//ourModel.ProcessAnimation();
		//ourModel.Draw(shader);

		depthMapShader.Bind();
		depthMapShader.SetUniform1f("near_plane", near);
		depthMapShader.SetUniform1f("far_plane", far);
		glActiveTexture(GL_TEXTURE0);
		/*depthFbo.Bind();*/
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderQuad();

		//render skybox
		//glDisable(GL_CULL_FACE);
		//skybox.Draw(camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.PrcoessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window, float time, ParticleSystem& ps)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		EmitParticle(ps, deltaTime);
}

void InitScene(Shader& shader, SkyBox skybox) {
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f));
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetView();

	glm::vec3 ambient(0.5f);
	glm::vec3 diffuse(0.6f);
	glm::vec3 specular(0.1f);
	
	shader.Bind();
	shader.SetUniformMat4f("model", model);
	shader.SetUniformMat4f("view", view);
	shader.SetUniformMat4f("projection", projection);

	//shader.SetUniform3f("dirLight.direction", 0.5f, -0.5f, -0.5f);
	shader.SetUniform3f("dirLight.lightPos", -2.0f, 5.0f, 8.0f);
	shader.SetUniformVec3("dirLight.ambient", ambient);
	shader.SetUniformVec3("dirLight.diffuse", diffuse);
	shader.SetUniformVec3("dirLight.specular", specular);

	shader.SetUniformVec3("viewPos", camera.GetPos());
	shader.SetUniform1i("skybox", 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexID());
}

void InitRole(Shader& shader) {
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f));
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetView();

	shader.Bind();
	shader.SetUniformMat4f("model", model);
	shader.SetUniformMat4f("view", view);
	shader.SetUniformMat4f("projection", projection);

	glm::vec3 ambient(0.5f);
	glm::vec3 diffuse(0.6f);
	glm::vec3 specular(0.1f);
	//shader.SetUniform3f("dirLight.direction", 5.0f, -5.0f, -5.0f);
	shader.SetUniform3f("dirLight.lightPos", -2.0f, 5.0f, 8.0f);
	shader.SetUniformVec3("dirLight.ambient", ambient);
	shader.SetUniformVec3("dirLight.diffuse", diffuse);
	shader.SetUniformVec3("dirLight.specular", specular);

	shader.SetUniform3f("viewPos", camera.GetPos().x, camera.GetPos().y, camera.GetPos().z);
}

void EmitParticle(ParticleSystem& ps, float time) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ParticleProps particle;
	particle.colorBegin = { Random::Float() * 0.2 + 0.8, Random::Float() * 0.3 + 0.7, Random::Float() * 0.9 + 0.1, 1.0f};
	particle.colorEnd = { Random::Float() * 0.2 + 0.8, Random::Float() * 0.3 + 0.7, Random::Float() * 0.9 + 0.1, 1.0f };
	particle.sizeBegin = 0.5f, particle.sizeVariation = 0.2f, particle.sizeEnd = 0.3f;
	particle.lifeTime = 1.0f;
	particle.velocity = { 0.0f, 0.0f, 0.0f };
	particle.velocityVariation = { Random::Float() * 10.0f, Random::Float() * 10.0f, Random::Float() * 10.0f };
	particle.position = { 0.0, 15.0f, -15.0f };
	glm::vec3 f(Random::Float() - 0.5, Random::Float() - 0.5, Random::Float() - 0.5);
	particle.F = 200.0f * glm::normalize(f);
	//particle.F = glm::vec3(1.0f);

	for (int i = 0; i < 10; i++) {
		ps.Emit(particle, time);
	}

}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
