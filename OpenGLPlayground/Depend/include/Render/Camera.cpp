#include "Camera.h"


Camera::Camera(float x, float y, float z, float targetX, float targetY, float targetZ) {
	this->cameraPos = glm::vec3(x, y, z);
	this->cameraTarget = glm::vec3(targetX, targetY, targetZ);
	this->init();
};

Camera::Camera(float x, float y, float z, float targetX, float targetY, float targetZ, float worldX, float worldY, float worldZ) {
	this->cameraPos = glm::vec3(x, y, z);
	this->cameraTarget = glm::vec3(targetX, targetY, targetZ);
	this->worldUp = glm::vec3(worldX, worldY, worldZ);
	this->init();
};

Camera::Camera(float x, float y, float z) {
	this->cameraPos = glm::vec3(x, y, z);
	this->init();
}

Camera::Camera() {
	this->init();
}


void Camera::init() {
	this->cameraDirection = glm::normalize(this->cameraPos - this->cameraTarget);
	this->cameraRight = glm::normalize(glm::cross(this->worldUp, this->cameraDirection));
	this->cameraUp = glm::cross(this->cameraDirection, this->cameraRight);

}

void Camera::translate(bool left, bool right, bool up, bool down, bool forward, bool backward) {
	glm::vec3 normed = glm::normalize(glm::cross(glm::cross(this->cameraFront, this->cameraUp), this->cameraFront));
	if (left)
		this->cameraPos -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * this->speed;
	if (right)
		this->cameraPos += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * this->speed;
	if (forward)
		this->cameraPos += this->speed * this->cameraFront;
	if (backward)
		this->cameraPos -= this->speed * this->cameraFront;
	if (up) {
		this->cameraPos += normed * this->speed;
	}
	if (down) {
		this->cameraPos -= normed * this->speed;
	}
}

void Camera::rotate(float xpos, float ypos, bool mousePressed) {
	this->xoffset = xpos - this->lastx;
	this->yoffset = this->lasty - ypos;
	this->lastx = xpos;
	this->lasty = ypos;
	xoffset *= this->sensitivity;
	yoffset *= this->sensitivity;
	if (mousePressed) {
		this->yaw += xoffset;
		this->pitch += yoffset;
		if (this->pitch > 89.9)
			this->pitch = 89.9;
		if (this->pitch < -89.9)
			this->pitch = -89.9;
		this->cameraDirection.x = glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
		this->cameraDirection.y = glm::sin(glm::radians(this->pitch));
		this->cameraDirection.z = glm::sin(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
		this->cameraFront = glm::normalize(this->cameraDirection);

	}
}

void Camera::zoom(float xoffset, float yoffset) {
	this->fov -= yoffset * this->zoomSensitivity;
	if (this->fov < 1.0f)
		this->fov = 1.0f;
	if (this->fov > 120.0f)
		this->fov = 120.0f;
}