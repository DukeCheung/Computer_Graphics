#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imconfig.h>
#include <imgui/imstb_textedit.h>
#include <imgui/imstb_rectpack.h>
#include <imgui/imstb_truetype.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "Camera.h"

using namespace std;

const char *GouraudVertexSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"out vec3 LightingColor;\n"

"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;"
"uniform vec3 lightColor;"

"uniform float ambientStrength;\n"
"uniform float diffuseStrength;\n"
"uniform float specularStrength;\n"
"uniform int reflectance;\n"

"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"

"   vec3 Position = vec3(model * vec4(aPos, 1.0));\n"
"   vec3 Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"   vec3 ambient = ambientStrength * lightColor;\n"
"   vec3 norm = normalize(Normal);\n"
"   vec3 lightDir = normalize(lightPos - Position);\n"
"   float diff = max(dot(norm, lightDir), diffuseStrength);\n"
"   vec3 diffuse = diff * lightColor;\n"
"   vec3 viewDir = normalize(viewPos - Position);\n"
"   vec3 reflectDir = reflect(-lightDir, norm);\n"
"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), reflectance);\n"
"   vec3 specular = specularStrength * spec * lightColor;\n"
"   LightingColor = ambient + diffuse + specular;\n"
"}\0";

const char *GouraudFragmentSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 LightingColor;\n"
"uniform vec3 objectColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(LightingColor * objectColor, 1.0);\n"
"}\n\0";

const char *PhongVertexSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"out vec3 Normal;\n"
"out vec3 FragPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"}\0";

const char *PhongFragmentSource = "#version 330 core\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"out vec4 FragColor;\n"

"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 objectColor;\n"

"uniform float ambientStrength;\n"
"uniform float diffuseStrength;\n"
"uniform float specularStrength;\n"
"uniform int reflectance;\n"

"void main()\n"
"{\n"
"   vec3 ambient = ambientStrength * lightColor;\n"
"   vec3 norm = normalize(Normal);\n"
"   vec3 lightDir = normalize(lightPos - FragPos);\n"
"   float diff = max(dot(norm, lightDir), diffuseStrength);\n"
"   vec3 diffuse = diff * lightColor;\n"
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 reflectDir = reflect(-lightDir, norm);\n"
"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), reflectance);\n"
"   vec3 specular = specularStrength * spec * lightColor;\n"
"   vec3 result = (ambient + diffuse + specular) * objectColor;\n"
"   FragColor = vec4(result, 1.0);\n"
"}\n\0";

const char *lightVertexSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";

const char *lightFragmentSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0);\n"
"}\n\0";

const char* glsl_version = "#version 130";

const int width = 800;
const int height = 800;
const float SPEED = 10.0f;
const float MOUSESPEED = 0.2f;
float lastX = width / 2.0f;
float lastY = height / 2.0f;

Camera camera(glm::vec3(4.0f, 4.0f, 10.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, -1.0f));

bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

void drawCube(const unsigned int shaderProgram);
void Projection(GLFWwindow *window, const unsigned int shaderProgram);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int createShader(const char *vertexShader, const char *fragment);
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "Cube", NULL, NULL);
	if (window == NULL) {
		cout << "´´½¨GLFWÊ§°Ü£¡" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "³õÊ¼»¯Ê§°Ü£¡" << endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	unsigned int lightShader, GouraudShader, PhongShader;
	lightShader = createShader(lightVertexSource, lightFragmentSource);
	GouraudShader = createShader(GouraudVertexSource, GouraudFragmentSource);
	PhongShader = createShader(PhongVertexSource, PhongFragmentSource);

	gladLoadGL();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	float vertices[] = {
	   -2.0f, -2.0f, -2.0f, 0.0f, 0.0f, -1.0f,
		2.0f, -2.0f, -2.0f, 0.0f, 0.0f, -1.0f,
		2.0f,  2.0f, -2.0f, 0.0f, 0.0f, -1.0f,
		2.0f,  2.0f, -2.0f, 0.0f, 0.0f, -1.0f,
	   -2.0f,  2.0f, -2.0f, 0.0f, 0.0f, -1.0f,
	   -2.0f, -2.0f, -2.0f, 0.0f, 0.0f, -1.0f,

	   -2.0f, -2.0f,  2.0f, 0.0f, 0.0f, 1.0f,
		2.0f, -2.0f,  2.0f, 0.0f, 0.0f, 1.0f,
		2.0f,  2.0f,  2.0f, 0.0f, 0.0f, 1.0f,
		2.0f,  2.0f,  2.0f, 0.0f, 0.0f, 1.0f,
	   -2.0f,  2.0f,  2.0f, 0.0f, 0.0f, 1.0f,
	   -2.0f, -2.0f,  2.0f, 0.0f, 0.0f, 1.0f,

	   -2.0f,  2.0f,  2.0f, -1.0f, 0.0f, 0.0f,
	   -2.0f,  2.0f, -2.0f, -1.0f, 0.0f, 0.0f,
	   -2.0f, -2.0f, -2.0f, -1.0f, 0.0f, 0.0f,
	   -2.0f, -2.0f, -2.0f, -1.0f, 0.0f, 0.0f,
	   -2.0f, -2.0f,  2.0f, -1.0f, 0.0f, 0.0f,
	   -2.0f,  2.0f,  2.0f, -1.0f, 0.0f, 0.0f,

		2.0f,  2.0f,  2.0f, 1.0f, 0.0f, 0.0f,
		2.0f,  2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, -2.0f,  2.0f, 1.0f, 0.0f, 0.0f,
		2.0f,  2.0f,  2.0f, 1.0f, 0.0f, 0.0f,

	   -2.0f, -2.0f, -2.0f, 0.0f, -1.0f, 0.0f,
		2.0f, -2.0f, -2.0f, 0.0f, -1.0f, 0.0f,
		2.0f, -2.0f,  2.0f, 0.0f, -1.0f, 0.0f,
		2.0f, -2.0f,  2.0f, 0.0f, -1.0f, 0.0f,
	   -2.0f, -2.0f,  2.0f, 0.0f, -1.0f, 0.0f,
	   -2.0f, -2.0f, -2.0f, 0.0f, -1.0f, 0.0f,

	   -2.0f,  2.0f, -2.0f, 0.0f, 1.0f, 0.0f,
		2.0f,  2.0f, -2.0f, 0.0f, 1.0f, 0.0f,
		2.0f,  2.0f,  2.0f, 0.0f, 1.0f, 0.0f,
		2.0f,  2.0f,  2.0f, 0.0f, 1.0f, 0.0f,
	   -2.0f,  2.0f,  2.0f, 0.0f, 1.0f, 0.0f,
	   -2.0f,  2.0f, -2.0f, 0.0f, 1.0f, 0.0f,
	};
	unsigned int VAO, VBO;
	
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	bool ImGui = true;
	bool Phong_Gouraud = true;
	bool bonus = false;
	float ambientStrength = 0.1;
	float diffuseStrength = 0.0;
	float specularStrength = 0.5;
	int reflectance = 32;
	float radius = 3.0f;
	glm::vec3 ObjectColor(1.0f, 0.5f, 0.31f);

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//background color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//depth test
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::vec3 lightPos(radius, radius, 0.0f);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Edit Radius", &ImGui, ImGuiWindowFlags_MenuBar);
		ImGui::Checkbox("Phong_Gouraud", &Phong_Gouraud);
		ImGui::Checkbox("Bonus", &bonus);
		ImGui::ColorEdit3("Color", (float*)&ObjectColor);
		ImGui::SliderFloat("Radius", &radius, 3.0f, 10.0f);
		ImGui::SliderFloat("Ambient", &ambientStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse", &diffuseStrength, 0.0f, 5.0f);
		ImGui::SliderFloat("Specular", &specularStrength, 0.0f, 1.0f);
		ImGui::SliderInt("Reflectance", &reflectance, 2, 256);
		ImGui::End();

		glm::mat4 lightModel = model;
		if (bonus) {
			float currentAngle = (float)glfwGetTime()*glm::radians(45.0f);
			lightModel = glm::rotate(model, currentAngle, glm::vec3(0.0f, 0.0f, 1.0f));
			lightModel = glm::translate(lightModel, glm::vec3(radius, 0.0f, 0.0f));
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			lightPos.x = cos(currentAngle)*radius;
			lightPos.y = sin(currentAngle)*radius;
		}
		else {
			lightModel = glm::translate(lightModel, lightPos);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		}

		if (Phong_Gouraud) {
			glUseProgram(PhongShader);

			glUniform3fv(glGetUniformLocation(PhongShader, "objectColor"), 1, &ObjectColor[0]);
			glUniform3f(glGetUniformLocation(PhongShader, "lightColor"), 1.0f, 1.0f, 1.0f);
			glUniform3fv(glGetUniformLocation(PhongShader, "lightPos"), 1, &lightPos[0]);
			glUniform3fv(glGetUniformLocation(PhongShader, "viewPos"), 1, &camera.getPosition()[0]);

			glUniform1f(glGetUniformLocation(PhongShader, "ambientStrength"), ambientStrength);
			glUniform1f(glGetUniformLocation(PhongShader, "diffuseStrength"), diffuseStrength);
			glUniform1f(glGetUniformLocation(PhongShader, "specularStrength"), specularStrength);
			glUniform1i(glGetUniformLocation(PhongShader, "reflectance"), reflectance);

			glUniformMatrix4fv(glGetUniformLocation(PhongShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(glGetUniformLocation(PhongShader, "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(PhongShader, "projection"), 1, GL_FALSE, &projection[0][0]);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else {
			glUseProgram(GouraudShader);
			glUniform3fv(glGetUniformLocation(GouraudShader, "objectColor"), 1, &ObjectColor[0]);
			glUniform3f(glGetUniformLocation(GouraudShader, "lightColor"), 1.0f, 1.0f, 1.0f);
			glUniform3fv(glGetUniformLocation(GouraudShader, "lightPos"), 1, &lightPos[0]);
			glUniform3fv(glGetUniformLocation(GouraudShader, "viewPos"), 1, &camera.getPosition()[0]);

			glUniform1f(glGetUniformLocation(GouraudShader, "ambientStrength"), ambientStrength);
			glUniform1f(glGetUniformLocation(GouraudShader, "diffuseStrength"), diffuseStrength);
			glUniform1f(glGetUniformLocation(GouraudShader, "specularStrength"), specularStrength);
			glUniform1i(glGetUniformLocation(GouraudShader, "reflectance"), reflectance);

			glUniformMatrix4fv(glGetUniformLocation(GouraudShader, "model"), 1, GL_FALSE, &model[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(GouraudShader, "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(GouraudShader, "projection"), 1, GL_FALSE, &projection[0][0]);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glUseProgram(lightShader);
		glUniformMatrix4fv(glGetUniformLocation(lightShader, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniformMatrix4fv(glGetUniformLocation(lightShader, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightShader, "projection"), 1, GL_FALSE, &projection[0][0]);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

unsigned int createShader(const char *vertexSource, const char *fragmentSource) {
	//compile vertex & fragment shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	int  success;
	char log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		cout << "¶¥µã×ÅÉ«Æ÷±àÒëÊ§°Ü£¡\n" << log << endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		cout << "Æ¬¶Î×ÅÉ«Æ÷±àÒëÊ§°Ü£¡\n" << log << endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(deltaTime*SPEED);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveBack(deltaTime*SPEED);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveLeft(deltaTime*SPEED);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(deltaTime*SPEED);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.rotate(xOffset*MOUSESPEED, yOffset*MOUSESPEED);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processZoom(yoffset);
}
