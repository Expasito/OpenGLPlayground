#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <iostream>
#include "../utils/Utils.h"


int main() {
	int width = 800;
	int height = 800;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(width, height, "Game", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initalize GLAD\n");
		return 1;
	}

	glViewport(0, 0, width, height);


	// now load shaders
	unsigned int program = compileShader("shaders/vertex0.shader", "shaders/fragment0.shader");

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, mouseCallBack);
	//glfwSetMouseButtonCallback(window, mouseButtonCallBack);
	//glfwSetScrollCallback(window, scrollCallBack);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int positions;
	glGenBuffers(1, &positions);
	glBindBuffer(GL_ARRAY_BUFFER, positions);

	struct Triangle {
		glm::vec3 p1;
	};

	float data[] = {
		-1,-1,0,
		0,1,0,
		1,-1,0
	};

	glUseProgram(program);


	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	checkErrors();

	while (!glfwWindowShouldClose(window)) {

		glBindBuffer(GL_ARRAY_BUFFER, positions);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
		std::cout << "hello\n";



	}

}