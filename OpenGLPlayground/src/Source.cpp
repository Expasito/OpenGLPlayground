#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../utils/Utils.h"
#include <glm/gtc/type_ptr.hpp>


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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, mouseCallBack);
	//glfwSetMouseButtonCallback(window, mouseButtonCallBack);
	//glfwSetScrollCallback(window, scrollCallBack);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int positions;
	glGenBuffers(1, &positions);
	glBindBuffer(GL_ARRAY_BUFFER, positions);

	
	float vertices[] = {
		// front face
		-1,-1,-1,
		1,-1,-1,
		0,1,-1,

		// back face
		-1,-1,1,
		1,-1,1,
		0,1,1,

		// right face lower
		1,-1,-1,
		1,1,-1,
		1,-1,1,
		// right face upper
		1,-1,1,
		1,1,1,
		1,-1,1

	};

	struct Triangle {
		glm::vec3 p1;
	};

	//float data[] = {
	//	-1,-1,0,
	//	0,1,0,
	//	1,-1,0
	//};

	int first[] = {0};
	int count[] = {36};

	glUseProgram(program);


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);

	glm::mat4 proj = glm::perspective(glm::radians(80.0f), 800.0f / 800.0f, .01f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 1, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));


	glm::mat4 trans(1);
	glm::mat4 translate = glm::translate(trans, glm::vec3(.5, 0, 0));
	glm::mat4 rotate = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0, 1, 0));

	glm::mat4 model = rotate * translate;

	unsigned int projLoc, viewLoc, modelLoc;
	projLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");

	std::cout << "Proj: " << projLoc << " View: " << viewLoc << " Model: " << modelLoc << "\n";

	while (!glfwWindowShouldClose(window)) {

		glBindBuffer(GL_ARRAY_BUFFER, positions);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		
		glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(model));

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glMultiDrawArrays(GL_TRIANGLES, first, count, 1);

		glfwSwapBuffers(window);
		glfwPollEvents();
		//std::cout << "hello\n";


		checkErrors();

	}

}