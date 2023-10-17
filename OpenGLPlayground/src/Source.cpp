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



	std::vector<glm::vec3> verts;
	std::vector<uint32_t> inds;
	makeIBO(vertices, verticesSize/sizeof(float),  &verts,  &inds);

	Mesh mesh2;

	std::vector<glm::vec3>* verts_ = &verts;
	std::vector<uint32_t>* inds_ = &inds;

	mesh2.loadMeshData(&verts, &inds);
	//glBindBuffer(GL_ARRAY_BUFFER, mesh2.vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), glm::value_ptr(verts[0]), GL_DYNAMIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, mesh2.vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts_->size(), glm::value_ptr((*verts_)[0]), GL_DYNAMIC_DRAW);


	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh2.ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * inds.size(), &inds[0], GL_DYNAMIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh2.ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * inds_->size(), &(*inds_)[0], GL_DYNAMIC_DRAW);



	std::vector<glm::vec3> verts2;
	verts2.push_back({ -1, -1, -1 });
	verts2.push_back({ -1,1,-1 });
	verts2.push_back({ 1,-1,-1 });

	std::vector<uint32_t> inds2;
	inds2.push_back(0);
	inds2.push_back(1);
	inds2.push_back(2);

	Mesh mesh;
	
	mesh.loadMeshData(&verts2, &inds2);





	glEnableVertexAttribArray(0);





	glm::mat4 trans(1);

	glm::mat4 proj = glm::perspective(glm::radians(80.0f), (float)width/height, 0.0f, 10000.0f);
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
	models_.push_back(glm::translate(trans, glm::vec3(-5, -5, -5)));
	models_.push_back(glm::translate(trans, glm::vec3(5, 5, 5)));
	models_.push_back(glm::rotate(trans, glm::radians(30.0f), glm::vec3(1, 0, 0)));

	for (int i = 0; i < 100000; i++) {
		models_.push_back(glm::translate(trans, glm::vec3(rand() % 10, rand() % 10, rand() % 10)));
	}


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

	// record how many triangles have been drawn
	int triangles = 0;

	// record how many draw calls have been called
	int drawCalls = 0;

	initProfile(10,10000, true);

	glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(proj));

	GLsizei counts[] = { inds.size(), inds.size(), inds.size() - 18};
	GLvoid* starts[] = { (GLvoid*)0, (GLvoid*)0, (GLvoid*)(NULL + 18*sizeof(unsigned int))};


	GLsizei counts2[] = { indices2Size, indices2Size, indices2Size };
	GLvoid* starts2[] = { (GLvoid*)0 , (GLvoid*)0 , (GLvoid*)0 };


	GLsizei c[] = { 3, 3, 3 };
	GLvoid* s[] = { (GLvoid*)(rand() % 5) , (GLvoid*)(rand() % 5) , (GLvoid*)(rand() % 5)};

	while (!glfwWindowShouldClose(window)) {
		start = std::chrono::high_resolution_clock::now();

		// reset counters
		objects = 0;
		drawCalls = 0;
		triangles = 0;


		// clear the buffer
		glClear(GL_COLOR_BUFFER_BIT);

		glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));

		//// bind vbo and ebo
		//glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
		//// send data over
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		//objects += models_.size();

		//glMultiDrawElements(GL_TRIANGLES, counts, GL_UNSIGNED_INT, (const void**)(starts), 3);
		//drawCalls++;

		//glBindBuffer(GL_ARRAY_BUFFER, v2);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);

		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		//glMultiDrawElements(GL_TRIANGLES, counts2, GL_UNSIGNED_INT, (const void**)starts2, 3);
		//drawCalls++;

		//glMultiDrawElements(GL_TRIANGLES, counts2, GL_UNSIGNED_INT, (const void**)starts2, 1);
		//drawCalls++;

		//glMultiDrawElements(GL_TRIANGLES, counts2, GL_UNSIGNED_INT, (const void**)starts2, 1);
		//drawCalls++;

		//checkErrors();


		

		//// simulate a different batch with different buffer objects
		//for (int batch = 0; batch < 100000; batch++) {
		//	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

		//	//glDrawElements(GL_TRIANGLES, mesh.indicesBufferSize, GL_UNSIGNED_INT, 0);
		//	glMultiDrawElements(GL_TRIANGLES, c, GL_UNSIGNED_INT, s, 3);

		//}

		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(glm::translate(trans, {1,2,1})));



		glBindBuffer(GL_ARRAY_BUFFER, mesh2.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh2.ibo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		glDrawElements(GL_TRIANGLES, mesh2.indicesBufferSize, GL_UNSIGNED_INT, 0);


		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(glm::translate(trans, { -4,2,1 })));


		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		glDrawElements(GL_TRIANGLES, mesh.indicesBufferSize, GL_UNSIGNED_INT, 0);

		

		//triangles += indices2Size / 3;
		//triangles += indexDataSize / 3 * models_.size();


		//std::cout << "Triangles: " << triangles << "\n";
		postRenderingSteps(false, window, &start, &proj, &view, width, height);
	}

	cleanup();

}