#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imconfig.h>
#include <imgui/imstb_textedit.h>
#include <imgui/imstb_rectpack.h>
#include <imgui/imstb_truetype.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

using namespace std;

const char *vertexSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 outColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"   outColor = aColor;\n"
"}\0";

const char *fragmentSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 outColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(outColor, 1.0);\n"
"}\n\0";

const char* glsl_version = "#version 130";

const int width = 800;
const int height = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

void drawCube(const unsigned int shaderProgram);

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

	//ImGui
	gladLoadGL();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	bool ImGui = true;
	bool bonus = false;
	bool depthTest = false;

	//initial angle and scale
	float angle = 90.0f;
	float scale = 50.0f;

	int speed = 0;
	//it shows the direction is left or right
	int direction = 0;
	
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Edit Radius", &ImGui, ImGuiWindowFlags_MenuBar);
		//the range of angle is from -360 to 360
		ImGui::SliderFloat("Angle", &angle, -360.0f, 360.0f);
		//the range of scale is from 35 to 100
		ImGui::SliderFloat("Scale", &scale, 35.0f, 100.0f);
		ImGui::Checkbox("Depth Test", &depthTest);
		ImGui::Checkbox("Bonus", &bonus);
		ImGui::End();
		
		//background color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//depth test
		if (depthTest) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glClear(GL_COLOR_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

		//move the cube on the horizontal direction
		model = glm::translate(model, (float)speed*glm::vec3(0.01f, 0.0f, 0.0f));
		if (speed >= 300) {
			direction = 1;
		}
		if (speed <= -300) {
			direction = 0;
		}
		//move to the right or left
		if (direction == 0) {
			speed++;
		}
		else {
			speed--;
		}

		//rotate the cube
		model = glm::rotate(model, (float)glfwGetTime()*glm::radians(angle), glm::vec3(1.0f, 0.0f, 1.0f));
		//the camera position
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
		//uniform scale
		projection = glm::perspective(glm::radians(scale), (float)height / (float)width, 0.1f, 100.0f);

		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

		drawCube(shaderProgram);
		if (bonus) {
			glm::mat4 modelBonus1 = glm::mat4(1.0f);
			glm::mat4 modelBonus2 = glm::mat4(1.0f);
			modelBonus1 = glm::translate(modelBonus1, glm::vec3(0.0f, 0.0f, 0.0f));
			modelBonus2 = glm::translate(modelBonus2, glm::vec3(0.0f, 0.0f, 0.0f));
			modelBonus1 = glm::rotate(modelBonus1, (float)glfwGetTime()*glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
			modelBonus1 = glm::translate(modelBonus1, glm::vec3(0.0f, 10.0f, 0.0f));
			modelBonus2 = glm::rotate(modelBonus2, (float)glfwGetTime()*glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
			modelBonus2 = glm::translate(modelBonus2, glm::vec3(10.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBonus1));
			drawCube(shaderProgram);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBonus2));
			drawCube(shaderProgram);
		}
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void drawCube(const unsigned int shaderProgram) {
	float vertices[] = {
	   -2.0f, -2.0f, -2.0f, 0.5f, 0.0f, 0.0f,
		2.0f, -2.0f, -2.0f, 0.5f, 0.0f, 0.0f,
		2.0f,  2.0f, -2.0f, 0.5f, 0.0f, 0.0f,
		2.0f,  2.0f, -2.0f, 0.5f, 0.0f, 0.0f,
	   -2.0f,  2.0f, -2.0f, 0.5f, 0.0f, 0.0f,
	   -2.0f, -2.0f, -2.0f, 0.5f, 0.0f, 0.0f,

	   -2.0f, -2.0f,  2.0f, 0.0f, 0.5f, 0.0f,
		2.0f, -2.0f,  2.0f, 0.0f, 0.5f, 0.0f,
		2.0f,  2.0f,  2.0f, 0.0f, 0.5f, 0.0f,
		2.0f,  2.0f,  2.0f, 0.0f, 0.5f, 0.0f,
	   -2.0f,  2.0f,  2.0f, 0.0f, 0.5f, 0.0f,
	   -2.0f, -2.0f,  2.0f, 0.0f, 0.5f, 0.0f,

	   -2.0f,  2.0f,  2.0f, 0.0f, 0.0f, 0.5f,
	   -2.0f,  2.0f, -2.0f, 0.0f, 0.0f, 0.5f,
	   -2.0f, -2.0f, -2.0f, 0.0f, 0.0f, 0.5f,
	   -2.0f, -2.0f, -2.0f, 0.0f, 0.0f, 0.5f,
	   -2.0f, -2.0f,  2.0f, 0.0f, 0.0f, 0.5f,
	   -2.0f,  2.0f,  2.0f, 0.0f, 0.0f, 0.5f,

		2.0f,  2.0f,  2.0f, 0.5f, 0.5f, 0.0f,
		2.0f,  2.0f, -2.0f, 0.5f, 0.5f, 0.0f,
		2.0f, -2.0f, -2.0f, 0.5f, 0.5f, 0.0f,
		2.0f, -2.0f, -2.0f, 0.5f, 0.5f, 0.0f,
		2.0f, -2.0f,  2.0f, 0.5f, 0.5f, 0.0f,
		2.0f,  2.0f,  2.0f, 0.5f, 0.5f, 0.0f,

	   -2.0f, -2.0f, -2.0f, 0.5f, 0.0f, 0.5f,
		2.0f, -2.0f, -2.0f, 0.5f, 0.0f, 0.5f,
		2.0f, -2.0f,  2.0f, 0.5f, 0.0f, 0.5f,
		2.0f, -2.0f,  2.0f, 0.5f, 0.0f, 0.5f,
	   -2.0f, -2.0f,  2.0f, 0.5f, 0.0f, 0.5f,
	   -2.0f, -2.0f, -2.0f, 0.5f, 0.0f, 0.5f,

	   -2.0f,  2.0f, -2.0f, 0.0f, 0.5f, 0.5f,
		2.0f,  2.0f, -2.0f, 0.0f, 0.5f, 0.5f,
		2.0f,  2.0f,  2.0f, 0.0f, 0.5f, 0.5f,
		2.0f,  2.0f,  2.0f, 0.0f, 0.5f, 0.5f,
	   -2.0f,  2.0f,  2.0f, 0.0f, 0.5f, 0.5f,
	   -2.0f,  2.0f, -2.0f, 0.0f, 0.5f, 0.5f
	};
	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
