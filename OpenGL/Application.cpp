#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <irrKlang.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Camera.h"
#include "Model.h"
#include "CheckError.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void InitScene(Shader& shader, Shader* noTexShader);

int BoneNum = 0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
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

	stbi_set_flip_vertically_on_load(false);
	Shader sceneShader("res/shader/Vertex.shader", "res/shader/Fragment.shader");
	Shader noTexShader("res/shader/Vertex.shader", "res/shader/NoTexFragment.shader");
	Shader shader("res/shader/basic.shader");
	Model ourModel("res/model/eula/eula.pmx");
	//Model scene("res/model/scene/scene.pmx");

	glEnable(GL_DEPTH_TEST);

	ourModel.SetStartTime();
	soundEngine->play2D("res/audio/music.mp3", GL_TRUE);

	while (!glfwWindowShouldClose(window)) {
		//glClearColor(0.0, 0.0, 0.0, 1.0);
		glClearColor(0.1, 0.5, 0.6, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		processInput(window);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.ProcessKeyboard(window, deltaTime);

		/*InitScene(sceneShader, &noTexShader);
		scene.Draw(sceneShader, &noTexShader);*/

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
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
		shader.SetUniform3f("dirLight.direction", 0.5f, 0.5f, -0.5f);
		shader.SetUniformVec3("dirLight.ambient", ambient);
		shader.SetUniformVec3("dirLight.diffuse", diffuse);
		shader.SetUniformVec3("dirLight.specular", specular);

		shader.SetUniform3f("viewPos", camera.GetPos().x, camera.GetPos().y, camera.GetPos().z);

		ourModel.ProcessAnimation();
		ourModel.Draw(shader);

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

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void InitScene(Shader& shader, Shader* noTexShader) {
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetView();

	shader.Bind();
	shader.SetUniformMat4f("model", model);
	shader.SetUniformMat4f("view", view);
	shader.SetUniformMat4f("projection", projection);

	noTexShader->Bind();
	shader.SetUniformMat4f("model", model);
	shader.SetUniformMat4f("view", view);
	shader.SetUniformMat4f("projection", projection);
}
