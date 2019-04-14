#include "Camera.h"

using namespace std;

Camera::Camera() {
	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Up = glm::vec3(0.0f, 0.0f, 0.0f);
	Front = glm::vec3(0.0f, 0.0f, 0.0f);
	zoom = 45.0f;
	yaw = -90.0f;
	pitch = 0.0f;
	updateVectors();
}
Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front) {
	Position = position;
	Up = up;
	Front = front;
	zoom = 45.0f;
	yaw = -90.0f;
	pitch = 0.0f;
	updateVectors();
}
Camera::~Camera(){}

void Camera::setPos(glm::vec3 position) {
	Position = position;
}

void Camera::setUp(glm::vec3 up) {
	Up = up;
}

void Camera::setFront(glm::vec3 front) {
	Front = front;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::moveForward(GLfloat const distance) {
	Position += Front * distance;
}
void Camera::moveBack(GLfloat const distance) {
	Position -= Front * distance;
}
void Camera::moveRight(GLfloat const distance) {
	Position += Right * distance;
}
void Camera::moveLeft(GLfloat const distance) {
	Position -= Right * distance;
}

void Camera::updateVectors() {
	glm::vec3 vectors;
	vectors.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	vectors.y = sin(glm::radians(pitch));
	vectors.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front = glm::normalize(vectors);
	Right = glm::normalize(glm::cross(Front, Up));
	cameraUp = glm::normalize(glm::cross(Right, Front));
}

void Camera::rotate(GLfloat const pitch, GLfloat const yaw) {
	this->pitch += pitch;
	this->yaw += yaw;
	if (this->pitch > 89.0f) {
		this->pitch = 89.0f;
	}
	if (this->pitch < -89.0f) {
		this->pitch = -89.0f;
	}
	updateVectors();
}

void Camera::processZoom(GLfloat const zoom) {
	if (this->zoom >= 1.0f && this->zoom <= 45.0f)
		this->zoom -= zoom;
	if (this->zoom <= 1.0f)
		this->zoom = 1.0f;
	if (this->zoom >= 45.0f)
		this->zoom = 45.0f;
}

GLfloat Camera::getZoom() {
	return zoom;
}