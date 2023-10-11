#include "Includes.h"
const int width = 800;
const int height = 800;



int main() {
	initRenderer(width, height);
	glUseProgram(program);



	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int positions;
	glGenBuffers(1, &positions);
	glBindBuffer(GL_ARRAY_BUFFER, positions);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	int first[] = {0};
	int count[] = {indexDataSize};

	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_DYNAMIC_DRAW);

	unsigned int test[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(test), test, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);


	glm::vec3 a = { 2,3,4 };
	glm::vec3 b = { 2,3,4 };
	std::cout << (a == b) << "\n";

	makeIBO();

	std::cout << "Vertex Data\n";
	for (int i = 0; i < vertexData.size(); i++) {
		glm::vec3 temp = vertexData.at(i);
		std::cout << "" << temp.x << ", " << temp.y << ", " << temp.z << "\n";
	}
	std::cout << "\nIBO Data:\n";
	for (int i = 0; i < indexDataSize; i++) {
		std::cout << indexData[i] << "\n";
	}

	//exit(1);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexDataSize, indexData, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexData.size(), &vertexData[0], GL_DYNAMIC_DRAW);

	std::cout << indexDataSize << "\n";
	//exit(1);
	glm::mat4 trans(1);
	std::vector<glm::mat4> translations;
	for (int i = 0; i < 1; i++) {
		translations.push_back(glm::translate(trans, glm::vec3(i, 0, 0)));

	}

	glm::mat4 proj = glm::perspective(glm::radians(80.0f), (float)width/height, .01f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 1, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));


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

	for (int i = 0; i < 35; i++) {
		unsigned int texture0 = loadTexture("textures/GrassBlock.png",
			GL_MIRRORED_REPEAT,
			GL_MIRRORED_REPEAT,
			GL_LINEAR_MIPMAP_LINEAR,
			GL_LINEAR);

		//std::cout << "Texture: " << texture0 << "\n";
		glBindTextureUnit(0, texture0);
		checkErrors();
		
	}

	


	unsigned int texture1 = loadTexture("textures/Blood.png",
		GL_MIRRORED_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR);
	checkErrors();

	//exit(1);

	int texture_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);

	std::cout << "Texture units: " << texture_units << "\n";

	int elements = 0;

	initProfile(10,10000, true);

	glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(proj));

	GLsizei counts[] = { indexDataSize};
	GLvoid* starts[] = { (GLvoid*)0};

	while (!glfwWindowShouldClose(window)) {
		start = std::chrono::high_resolution_clock::now();
		elements = 0;


		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, positions);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		//rotate = glm::rotate(trans, glm::radians(cntr), glm::vec3(0, 1, 0));
		//cntr += .1;
		glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));

		for (int i = 0; i < translations.size(); i++) {
			//model = translations.at(i) * rotate;
			model = translations.at(i);

			glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(model));


			//glMultiDrawArrays(GL_TRIANGLES, first, count, 1);
			//glDrawElements(GL_TRIANGLES, count[0], GL_UNSIGNED_INT, NULL);
			//glDrawElements(GL_TRIANGLES, indexDataSize, GL_UNSIGNED_INT, NULL);
			//glMultiDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (const void*const*)indexData, 1);
			// This needs to be changed to be outside of the loop and have the loop assemble a vbo 
			glMultiDrawElements(GL_TRIANGLES, counts, GL_UNSIGNED_INT, (const void**)(starts), 1);
			checkErrors();

			elements++;
		}

		

		//std::cout << "Elements drawn: " << elements << "\n";
		postRenderingSteps(false, window, &start, &proj, &view, width, height);
		
	}

	cleanup();

}