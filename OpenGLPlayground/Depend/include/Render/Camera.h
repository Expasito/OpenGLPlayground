#pragma once
#include "Headers.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
public:
	//Camera details
	float fov = 80.0f;
	float sensitivity = .4f;
	float yaw = 90.0f;
	float pitch = 0.0f;
	float roll = 0.0f;
	float baseSpeed = 10.0f;
	float speed = baseSpeed;
	float zoomSensitivity = 3.0f;
	//Camera vectors
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;

	//mouse position data
	float lastx = 400, lasty = 300;
	float xoffset = 0, yoffset = 0;


	Camera(float x, float y, float z, float targetX, float targetY, float targetZ);
	Camera(float x, float y, float z, float targetX, float targetY, float targetZ, float worldX, float worldY, float worldZ);
	Camera(float x, float y, float z);
	Camera();

	void translate(bool left, bool right, bool up, bool down, bool forward, bool backward);
	void rotate(float xpos, float ypos, bool mousePressed);
	void zoom(float xoffset, float yoffset);


private:
	//Basic after constructor function
	void init();
};

