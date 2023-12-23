#include "Includes.h"
#include <glm/gtx/string_cast.hpp>

const int width = 800;
const int height = 800;


/*
* Define a few cout operators to make debugging easier
*/

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
	os << vec.x << " " << vec.y << " " << vec.z << " ";
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec)
{
	os << vec.x << " " << vec.y << " ";
	return os;
}


/*
* 
* Simple Model struct which holds components and can print them
* 
*/

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

	// enable certain vertex attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	float data[] = {
		-1.0,-1.0,-1.0, -1.0,-1.0,-1.0, 0,0,
		-1.0,1.0,-1.0, -1.0,1.0,-1.0, 0.0,1.0,
		1.0,-1.0,-1.0, 1.0,-1.0,-1.0, 1.0,0.0,

		-1.0,1.0,-1.0, -1.0,1.0,-1.0, 0.0,1.0,
		1.0,-1.0,-1.0, 1.0,-1.0,-1.0, 1.0,0.0,
		1.0,1.0,-1.0, 1.0,1.0,-1.0, 1.0,1.0
	};


	std::vector<Vertex> verts1;
	std::vector<uint32_t> inds1;
	makeIBO(data, sizeof(data)/sizeof(float), &verts1, &inds1);

	std::vector<Vertex> verts2;
	std::vector<uint32_t> inds2;
	makeIBO(cubeVertices, cubeVertsSize / sizeof(float), &verts2, &inds2);

	Mesh mesh1;
	Mesh mesh2;


	// create our two meshes to use
	mesh1.loadMeshData(&verts2, &inds2);
	mesh2.loadMeshData(&verts1, &inds1);


	/*
	* Load textures here
	*/
	for (int i = 0; i < 35; i++) {
		unsigned int texture0 = loadTexture("textures/GrassBlock.png",
			GL_MIRRORED_REPEAT,
			GL_MIRRORED_REPEAT,
			GL_LINEAR_MIPMAP_LINEAR,
			GL_LINEAR);
	}

	unsigned int texture0 = loadTexture("textures/GrassBlock.png",
		GL_MIRRORED_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR);


	unsigned int texture1 = loadTexture("textures/OldRock.png",
		GL_MIRRORED_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR);




	// now assign which textures belong to which index. So if the material
	// has index 0, then it gets the texture with that index associated with it
	textureMap[0] = texture0;
	textureMap[3] = texture1;


	// Create 3 materials to draw with
	// Order: Albedo, diffuse, specular, shininess, areTextures
	// If aretextures is 1, albedo.x is which texture to use
	Material m1({.5,.6,.7}, {.1,.5,.3}, {.5,.8,.3}, 100, 0);
	Material m2({0, .1,.3}, {.5,.5,.2}, {.2,.4,.2}, 10, 1);
	Material m3({ 3, .5,.5 }, { .5,.5,.2 }, { .2,.4,.2 }, 10, 1);




	// show axis
	glm::vec3 x = {1,0,0};
	glm::vec3 y = glm::cross(x, glm::vec3(0,0,-1));
	glm::vec3 z = glm::cross(x, y);

	// Create components for each axis
	Component axisX(&mesh1, &m1, x, { 0,0,0 }, { 10,.0125,.0125 });
	Component axisY(&mesh1, &m1, y, { 0,0,0 }, { .0125,10,.0125 });
	Component axisZ(&mesh1, &m1, z, { 0,0,0 }, { .0125,.0125,10 });




	Component p1(&mesh1, &m2, {5,5,5}, { 0,0,0 }, { 1,1,1 });

	// This is the model we are trying to build of the components
	Model axes;
	axes.add(&axisX);
	axes.add(&axisY);
	axes.add(&axisZ);

	Model model1;
	model1.add(&p1);








	// create our matricies for the camera
	glm::mat4 trans(1);
	glm::mat4 proj = glm::perspective(glm::radians(80.0f), (float)width/height, 0.0f, 10000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 1, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));



	// get the location for the matrices so we can send the data to the GPU
	unsigned int projLoc, viewLoc, modelLoc;
	projLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");


	// Define timers for our program so we can measure performance
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;


	// Yes we want to profile, and do it 10 frames after we start the program
	initProfile(10,10000, true);

	// set the camera up early
	updateProjView(&proj, &view, width, height);


	// assign these variables to the correct location for the shader
	// so the material objects can update them accordinly
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




	/*
	* 
	* Testing begins here
	* 
	*/


	// this will combine objects of the same material

	uint32_t batchBuffer;
	uint32_t batchIBO;
	glGenBuffers(1, &batchBuffer);
	glGenBuffers(1, &batchIBO);
	glBindBuffer(GL_ARRAY_BUFFER, batchBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (verts1.size() + verts2.size()), NULL, GL_DYNAMIC_DRAW);

	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * verts1.size(), &verts1[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * verts1.size(), sizeof(Vertex) * verts2.size(), &verts2[0]);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * (inds1.size() + inds2.size()), NULL , GL_DYNAMIC_DRAW);


	std::vector<uint32_t> cpy(inds2);
	// add the number of initial vertices to the index so we displace to the right locations
	int dx = verts1.size();
	for (int i = 0; i < cpy.size(); i++) {
		cpy[i] += dx;
	}
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * inds1.size(), &inds1[0]);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * inds1.size(),sizeof(uint32_t) * inds2.size(), &cpy[0]);





	uint32_t* cheese = new uint32_t[inds1.size() + inds2.size()];
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * (inds1.size() + inds2.size()), cheese);

	Vertex* bacon = new Vertex[verts1.size() + verts2.size()];
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * (verts1.size() + verts2.size()), bacon);

	for (int i = 0; i < inds1.size() + inds2.size(); i++) {
		std::cout << cheese[i] << " ";
	}
	std::cout << "\n";
	for (int i = 0; i < (verts1.size() + verts2.size()); i++) {
		Vertex v = bacon[i];
		std::cout << i << " : " << "(" << v.pos << ", " << v.normal << ", " << v.textCoord << ")\n";
	}






	// NEED TO FIGURE THIS OUT

	// record how many indicies to draw. This should be the indices count for each mesh
	std::vector<GLsizei> count__;
	count__.push_back(inds2.size());
	count__.push_back(inds2.size());
	count__.push_back(inds2.size());

	count__.push_back(inds1.size());
	count__.push_back(inds1.size());
	count__.push_back(inds1.size());


	// Give where to start in the buffer
	std::vector<GLvoid*> start__;
	start__.push_back((GLvoid*)(sizeof(uint32_t) * inds1.size()));
	start__.push_back((GLvoid*)(sizeof(uint32_t) * inds1.size()));
	start__.push_back((GLvoid*)(sizeof(uint32_t) * inds1.size()));
	start__.push_back(0);
	start__.push_back(0);
	start__.push_back(0);


	
	
	// This keeps track of all model matrices for Multidraw to use
	std::vector<glm::mat4> models_;

	models_.push_back(glm::translate(glm::mat4(1), glm::vec3(5, 0, 0)));
	models_.push_back(glm::translate(glm::mat4(1), glm::vec3(-5, 0, 0)));
	models_.push_back(glm::translate(glm::mat4(1), glm::vec3(0, 5, 0)));
	models_.push_back(glm::translate(glm::mat4(1), glm::vec3(0, -5, 0)));





	// Make model matrices and send them over
	unsigned int models;
	models = glGetUniformLocation(program, "models");
	glUniformMatrix4fv(models, models_.size(), GL_FALSE, glm::value_ptr(models_[0]));

	glm::mat4 model(1);



	float cntr = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		start = std::chrono::high_resolution_clock::now();

		// clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// this must be done each time
		glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));





		glBindBuffer(GL_ARRAY_BUFFER, mesh1.vbo);
		// give position, normal and texture coordinate data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) * 1));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) * 2));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh1.ibo);

		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));


		//GLvoid* a = (GLvoid*)(4);
		//glDrawElements(GL_TRIANGLES, inds2.size(), GL_UNSIGNED_INT, a);

		//glDrawElements(GL_TRIANGLES ,  inds.size() + inds2.size(), GL_UNSIGNED_INT, 0);

		//glMultiDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indexStart, 2);
		glMultiDrawElements(GL_TRIANGLES, &count__[0], GL_UNSIGNED_INT, &start__[0], count__.size());



		postRenderingSteps(false, window, &start, &proj, &view, width, height);
	}

	cleanup();

}