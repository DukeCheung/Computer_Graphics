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

using namespace std;

const int width = 800;
const int height = 800;
float currentX = 0.0f;
float currentY = 0.0f;
float vertices[200];
float qVertices[4];
int pointNum = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow* window, int button, int action, int mods);
void cursorCallback(GLFWwindow* window, double x, double y);
void renderPoint(unsigned int VAO);
void renderLine(unsigned int VAO);
int factorial(int n);
float binomialCoef(int n, int i);
void renderBezierCurve();

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "Bezier", NULL, NULL);
	if (window == NULL) {
		cout << "创建GLFW失败！" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "初始化失败！" << endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursorCallback);
	//glfwSetScrollCallback(window, scroll_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(window, mouseCallback);
	unsigned int VAO, VBO;

	while (!glfwWindowShouldClose(window)) {

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderPoint(VAO);
		renderLine(VAO);
		renderBezierCurve();
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			vertices[pointNum * 2] = currentX / (float)(width/2);
			vertices[pointNum * 2 + 1] = currentY / (float)(height / 2);
			pointNum++;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			pointNum--;
			break;
		default:
			break;
		}
	}
}
void cursorCallback(GLFWwindow* window, double x, double y) {
	currentX = x - width / 2;
	currentY = -y + height / 2;
}
void renderPoint(unsigned int VAO) {
	glBindVertexArray(VAO);
	glPointSize(10.0f);
	glDrawArrays(GL_POINTS, 0, pointNum);
}
void renderLine(unsigned int VAO) {
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, pointNum);
}


int factorial(int n) {
	if (n > 1) {
		return n * factorial(n - 1);
	}
	return 1;
}
float binomialCoef(int n, int i) {
	float coff;
	coff = (float)factorial(n) / (factorial(i)*factorial(n - i));
	return coff;
}
void renderBezierCurve() {
	qVertices[0] = vertices[0];
	qVertices[1] = vertices[1];

	for (float t = 0.0f; t < 1.0f; t += 0.02f) {
		for (int i = 0; i < pointNum; i++) {
			qVertices[2] += vertices[i * 2] * binomialCoef(pointNum - 1, i)*pow(t, i)*pow((1 - t), pointNum - 1 - i);
			qVertices[3] += vertices[i * 2 + 1] * binomialCoef(pointNum - 1, i)*pow(t, i)*pow((1 - t), pointNum - 1 - i);
		}
		unsigned int qVAO, qVBO;
		glGenBuffers(1, &qVBO);
		glBindBuffer(GL_ARRAY_BUFFER, qVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(qVertices), qVertices, GL_STATIC_DRAW);
		glGenVertexArrays(1, &qVAO);
		glBindVertexArray(qVAO);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		if (pointNum > 0) {
			glBindVertexArray(qVAO);
			glDrawArrays(GL_LINE_STRIP, 0, 2);
		}

		glDeleteVertexArrays(1, &qVAO);
		glDeleteBuffers(1, &qVBO);

		qVertices[0] = qVertices[2];
		qVertices[1] = qVertices[3];
		qVertices[2] = 0.0f;
		qVertices[3] = 0.0f;
	}
}
