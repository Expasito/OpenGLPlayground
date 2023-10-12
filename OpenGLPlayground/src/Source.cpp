#include "Includes.h"
const int width = 800;
const int height = 800;



int main() {
	initRenderer(width, height);
	glUseProgram(program);


	// create our VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// create our vertex position buffer
	unsigned int verticesBuffer;
	glGenBuffers(1, &verticesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);

	// and create our element index buffer
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	int first[] = {0};
	int count[] = {indexDataSize};

	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);

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


	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexDataSize, indexData, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexData.size(), &vertexData[0], GL_DYNAMIC_DRAW);

	glm::mat4 trans(1);

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

		glBindTextureUnit(i, texture0);
		checkErrors();
		
	}


	// Make model matrices and send them over
	unsigned int models;
	models = glGetUniformLocation(program, "models");
	std::vector<glm::mat4> models_;
	models_.push_back(glm::translate(trans, glm::vec3(1, 1, 1)));
	models_.push_back(glm::translate(trans, glm::vec3(4, 4, 1)) * glm::rotate(trans, glm::radians(30.0f), glm::vec3(1, 0, 0)));
	models_.push_back(glm::translate(trans, glm::vec3(1, -4, 1)));


	glUniformMatrix4fv(models, models_.size(), GL_FALSE, glm::value_ptr(models_[0]));


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

	// record how many objects have been drawn
	int objects = 0;

	// record how many draw calls have been called
	int drawCalls = 0;

	initProfile(10,10000, true);

	glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(proj));

	GLsizei counts[] = { indexDataSize, indexDataSize, indexDataSize-18};
	GLvoid* starts[] = { (GLvoid*)0, (GLvoid*)0, (GLvoid*)(NULL + 18*sizeof(unsigned int))};

	while (!glfwWindowShouldClose(window)) {
		start = std::chrono::high_resolution_clock::now();

		// reset counters
		objects = 0;
		drawCalls = 0;

		// clear the buffer
		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));


		objects += models_.size();

			glMultiDrawElements(GL_TRIANGLES, counts, GL_UNSIGNED_INT, (const void**)(starts), 3);
			drawCalls++;
			checkErrors();


		postRenderingSteps(false, window, &start, &proj, &view, width, height);
		
	}

	cleanup();

}