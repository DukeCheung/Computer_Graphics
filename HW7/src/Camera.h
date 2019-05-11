#ifndef CAMERA_H
#define CAMERA_H
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front);
	~Camera();
	void setPos(glm::vec3 position);
	void setUp(glm::vec3 up);
	void setFront(glm::vec3 front);

	glm::mat4 getViewMatrix();

	void moveForward(GLfloat const distance);
	void moveBack(GLfloat const distance);
	void moveRight(GLfloat const distance);
	void moveLeft(GLfloat const distance);
	void rotate(GLfloat const pitch, GLfloat const yaw);
	void processZoom(GLfloat const zoom);
	GLfloat getZoom();
	glm::vec3 getPosition();
private:
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Front;
	glm::vec3 cameraUp;
	glm::vec3 Right;
	GLfloat pitch;
	GLfloat yaw;
	GLfloat zoom;
	void updateVectors();
};
#endif // !CAMERA_H


