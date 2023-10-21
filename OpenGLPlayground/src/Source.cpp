#include "Includes.h"
const int width = 800;
const int height = 800;

struct Material {
	glm::vec3 albedo;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};


struct Component {
	Mesh* mesh;
	Material material;
	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scalate;
	glm::mat4 model;

	Component(Mesh* mesh, Material material, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scalate) {
		this->mesh = mesh;
		this->material = material;
		this->translate = translate;
		this->rotate = rotate;
		this->scalate = scalate;
		updateModelMatrix();
	}

	void updateModelMatrix() {
		glm::mat4 trans(1.0f);
		model = glm::scale(trans, scalate) *
			glm::rotate(trans, glm::radians(rotate.x), { 1,0,0 }) *
			glm::rotate(trans, glm::radians(rotate.y), { 0,1,0 }) *
			glm::rotate(trans, glm::radians(rotate.z), { 0,0,1 }) *
			glm::translate(trans, translate);
	}

	void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->ibo);

		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, mesh->indicesBufferSize, GL_UNSIGNED_INT, 0);
	}
};


struct Model {
	std::vector<Component*> components;

	void add(Component* m) {
		components.push_back(m);
	}
};



int main() {
	initRenderer(width, height);
	
	glUseProgram(program);
	glEnable(GL_DEPTH_TEST);


	// create our VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	std::vector<glm::vec3> verts;
	std::vector<uint32_t> inds;
	makeIBO(vertices, verticesSize/sizeof(float),  &verts,  &inds);

	Mesh mesh2;

	std::vector<glm::vec3>* verts_ = &verts;
	std::vector<uint32_t>* inds_ = &inds;

	mesh2.loadMeshData(&verts, &inds);



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


	Model m;

	Component comp(&mesh, { {1.0, .5, .3}, {.5,.5,.5}, {.25,.25,.25}, 100.0f }, { 4,0,0 }, { 0,90,0 }, { 2,1,1 });




	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);





	glm::mat4 trans(1);

	glm::mat4 proj = glm::perspective(glm::radians(80.0f), (float)width/height, 0.0f, 10000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 1, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));




	unsigned int projLoc, viewLoc, modelLoc;
	projLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");


	std::cout << "Proj: " << projLoc << " View: " << viewLoc << " Model: " << modelLoc << "\n";


	
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

	unsigned int texture0 = loadTexture("textures/GrassBlock.png",
		GL_MIRRORED_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR);

	glBindTextureUnit(0, texture0);

	unsigned int texture1 = loadTexture("textures/OldRock.png",
		GL_MIRRORED_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR);
	glBindTextureUnit(1,texture1);






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

	// send models to the gpu
	glUniformMatrix4fv(models, models_.size(), GL_FALSE, glm::value_ptr(models_[0]));


	//unsigned int texture1 = loadTexture("textures/StonePath.png",
	//	GL_MIRRORED_REPEAT,
	//	GL_MIRRORED_REPEAT,
	//	GL_LINEAR_MIPMAP_LINEAR,
	//	GL_LINEAR);


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


	GLsizei counts[] = { inds.size(), inds.size(), inds.size() - 18};
	GLvoid* starts[] = { (GLvoid*)0, (GLvoid*)0, (GLvoid*)(NULL + 18*sizeof(unsigned int))};


	GLsizei counts2[] = { indices2Size, indices2Size, indices2Size };
	GLvoid* starts2[] = { (GLvoid*)0 , (GLvoid*)0 , (GLvoid*)0 };


	GLsizei c[] = { 3, 3, 3 };
	GLvoid* s[] = { (GLvoid*)(rand() % 5) , (GLvoid*)(rand() % 5) , (GLvoid*)(rand() % 5)};

	// no texturing
	glUniform1i(glGetUniformLocation(program, "texturing"), 1);

	uint32_t textureCoords;
	float textureCoordsArray[] = {
		0, 0,
		0, 1,
		1, 0,
		1,1,
		0,1,
		1,1,
		0,0,
		1,0,
		1,1,
		0,1,
	};
	glGenBuffers(1, &textureCoords);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordsArray), textureCoordsArray, GL_STATIC_DRAW);

	// set the camera up early
	updateProjView(&proj, &view, width, height);


	Material mat = { {1,0,0},{0,0,0},{1,1,1},10.0 };


	uint32_t matAlbedo = glGetUniformLocation(program, "material.albedo");
	std::cout << "Location: " << matAlbedo << "\n";
	glUniform3fv(matAlbedo, 1, glm::value_ptr(glm::vec3(1, 0, 1)));


	uint32_t cubeBuff;
	glGenBuffers(1, &cubeBuff);
	glBindBuffer(GL_ARRAY_BUFFER, cubeBuff);
	glBufferData(GL_ARRAY_BUFFER, cubeVertsSize, cubeVertices, GL_DYNAMIC_DRAW);




	while (!glfwWindowShouldClose(window)) {
		start = std::chrono::high_resolution_clock::now();

		// reset counters
		objects = 0;
		drawCalls = 0;
		triangles = 0;


		// clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));



		glUniform1i(glGetUniformLocation(program, "texturing"), -1);

		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(glm::translate(glm::mat4(1), {-5,0,0})));
		glUniform3fv(matAlbedo, 1, glm::value_ptr(glm::vec3(1, 0, 1)));

		glBindBuffer(GL_ARRAY_BUFFER, mesh2.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh2.ibo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, textureCoords);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		glDrawElements(GL_TRIANGLES, mesh2.indicesBufferSize, GL_UNSIGNED_INT, 0);





		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(glm::translate(trans, { 0,0,0 })));
		glUniform3fv(matAlbedo, 1, glm::value_ptr(glm::vec3(0, 1, 1)));

		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		glBindBuffer(GL_ARRAY_BUFFER, textureCoords);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		glDrawElements(GL_TRIANGLES, mesh.indicesBufferSize, GL_UNSIGNED_INT, 0);


		glUniform1i(glGetUniformLocation(program, "texturing"), 1);
		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(glm::translate(trans, { 0,5,0 })));

		glBindBuffer(GL_ARRAY_BUFFER, cubeBuff);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		comp.draw();


		postRenderingSteps(false, window, &start, &proj, &view, width, height);
	}

	cleanup();

}