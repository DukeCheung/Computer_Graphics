#include "BresenHam.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imconfig.h>
#include <imgui/imstb_textedit.h>
#include <imgui/imstb_rectpack.h>
#include <imgui/imstb_truetype.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
using std::cin;
using std::cout;
using std::endl;

const int width = 800;
const int height = 800;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);
const char* glsl_version = "#version 130";
int main() {

	BresenHam m;
	m.input();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "Triangle", NULL, NULL);
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

	gladLoadGL();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImVec4 TriangleColor = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);

	bool ImGui = true;
	bool showTriangle = true;
	bool showCircle = false;
	bool fillTriangle = false;
	bool changeRadius = false;

	glEnable(GL_PROGRAM_POINT_SIZE);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Edit Radius", &ImGui, ImGuiWindowFlags_MenuBar);
		ImGui::ColorEdit3("Radius", (float*)&TriangleColor);
		ImGui::Checkbox("Triangle", &showTriangle);
		ImGui::Checkbox("Circle", &showCircle);
		ImGui::Checkbox("ChangeRadius", &changeRadius);
		ImGui::Checkbox("fillTriangle", &fillTriangle);
		ImGui::End();
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		if (showTriangle) {
			m.drawTriangle();
		}
		if (showCircle) {
			m.drawCircle();
			changeRadius = false;
		}
		if (changeRadius) {
			m.drawCircle((int)(TriangleColor.x*width/2));
			showCircle = false;
		}
		if (fillTriangle) {
			m.fillTriangle();
		}

		// Rendering
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

