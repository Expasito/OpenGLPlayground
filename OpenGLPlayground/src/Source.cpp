#include "Includes.h"
const int width = 800;
const int height = 800;








struct Model {
	std::vector<Component*> components;

	void add(Component* m) {
		components.push_back(m);
	}

	void draw() {
		for (Component* c : components) {
			c->draw();
		}
	}
};

struct Node {
	Model* model;
	
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

	//Material m1 = { {1.0, .5, .3}, {.5,.5,.5}, {.25,.25,.25}, 100.0f, 1 };
	//Material m2 = { {1,1,0},{.4,.2,.4},{.7,.6,.5},90,0 };

	Material m1({.5,.6,.7}, {.1,.5,.3}, {.5,.8,.3}, 100, 0);

	Material m2({1,0,0}, {2,0,0}, {0,0,0}, 10, 1);

	Component c1(&mesh, &m1, { 0,0,0 }, {90,0,0}, {1,2,1});
	Component c2(&mesh, &m2, {5,0,0}, {0,0,0}, {1,1,1});

	Component c3(&mesh2, &m1, {0,0,5}, {0,90,0}, {2,2,2});
	Component c4(&mesh2, &m2, {-5,-5,0}, {45,45,45}, {3,3,3});

	Model m;
	m.add(&c1);
	m.add(&c2);
	m.add(&c3);
	m.add(&c4);


	//Model m;

	//Component comp(&mesh,&m1, { 4,0,0 }, { 0,90,0 }, { 2,1,1 });

	//Component comp2(&mesh, 
	//	&m2, 
	//	{-4,-3,2},
	//	{0,45,30},
	//	{1,2,1}
	//);

	//Component comp3(&mesh2, &m1, { 0,0,0 }, { 0,0,0 }, { .5,.5,.5 });

	//m.add(&comp);
	//m.add(&comp2);
	//m.add(&comp3);



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


	//Material mat = { {1,0,0},{0,0,0},{1,1,1},10.0, 0 };


	matAlbedo = glGetUniformLocation(program, "material.albedo");
	matDiffuse = glGetUniformLocation(program, "material.diffuse");
	matSpecular = glGetUniformLocation(program, "material.specular");
	matShininess = glGetUniformLocation(program, "material.shininess");
	matAreTextures = glGetUniformLocation(program, "material.areTextures");

	std::cout << "Albedo: " << matAlbedo << "\n";
	std::cout << "Diffuse: " << matDiffuse << "\n";
	std::cout << "Specular: " << matSpecular << "\n";
	std::cout << "Shininess: " << matShininess << "\n";
	std::cout << "AreTextures: " << matAreTextures << "\n";

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








		// We need to standardize what is required for input vertices
		glUniform1i(glGetUniformLocation(program, "texturing"), 1);
		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(glm::translate(trans, { 0,5,0 })));

		glBindBuffer(GL_ARRAY_BUFFER, cubeBuff);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
		//glDrawArrays(GL_TRIANGLES, 0, 36);



		//c1.draw();
		//c2.draw();

		m.draw();

		//m1.drawAll();
		//m.draw();

		//comp.draw();
		//comp2.draw();

		//for (void* c : m.components) {
		//	((Component*)c)->draw();
		//}


		postRenderingSteps(false, window, &start, &proj, &view, width, height);
	}

	cleanup();

}