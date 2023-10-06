#include "Includes.h"
const int width = 800;
const int height = 800;



int main() {
	initRenderer(width, height);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int positions;
	glGenBuffers(1, &positions);
	glBindBuffer(GL_ARRAY_BUFFER, positions);

	


	int first[] = {0};
	int count[] = {35*3};

	glUseProgram(program);




	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);

	glm::mat4 proj = glm::perspective(glm::radians(80.0f), (float)width/height, .01f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 1, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));


	glm::mat4 trans(1);
	glm::mat4 translate = glm::translate(trans, glm::vec3(0, -4, 10));
	glm::mat4 rotate = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0, 1, 0));

	glm::mat4 model = translate * rotate;

	unsigned int projLoc, viewLoc, modelLoc;
	projLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");

	std::cout << "Proj: " << projLoc << " View: " << viewLoc << " Model: " << modelLoc << "\n";

	float cntr = 0.001f;
	
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;



	while (!glfwWindowShouldClose(window)) {
		start = std::chrono::high_resolution_clock::now();


		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, positions);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		rotate = glm::rotate(trans, glm::radians(cntr), glm::vec3(0, 1, 0));
		cntr += .1;

		model = translate * rotate;

		
		glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(model));

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glMultiDrawArrays(GL_TRIANGLES, first, count, 1);


		postRenderingSteps(window, &start, &proj, &view, width, height);
		
	}

}