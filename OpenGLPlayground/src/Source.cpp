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


class BoundingBox {
public:
	std::vector<Component*> components;

	// get the min and max of every direction
	float minX = 1e9, maxX = -1e9;
	float minY = 1e9, maxY = -1e9;
	float minZ = 1e9, maxZ = -1e9;

	void add(Component* c) {
		components.push_back(c);
		update();
	}

	void update() {
		// reset values
		minX = 1e9;
		maxX = -1e9;
		minY = 1e9;
		maxY = -1e9;
		minZ = 1e9;
		maxZ = -1e9;


		for (Component* c : components) {
			Mesh* m = c->mesh;
			std::vector<Vertex>* vs = m->verts;
			for (int i = 0; i < vs->size(); i++) {
				Vertex v = vs->at(i);
				glm::vec4 vertex = c->model * glm::vec4(v.pos,1.0f);
				if (vertex.x > maxX) {
					maxX = vertex.x;
				}
				if (vertex.x < minX) {
					minX = vertex.x;
				}
				if (vertex.y > maxY) {
					maxY = vertex.y;
				}
				if (vertex.y < minY) {
					minY = vertex.y;
				}
				if (vertex.z > maxZ) {
					maxZ = vertex.z;
				}
				if (vertex.z < minZ) {
					minZ = vertex.z;
				}
			}
		}
	}
private:

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

	float data2[] = {
		-1.0,-1.0,1.0, -1.0,-1.0,-1.0, 0,0,
		-1.0,1.0,1.0, -1.0,1.0,-1.0, 0.0,1.0,
		1.0,-1.0,1.0, 1.0,-1.0,-1.0, 1.0,0.0,

		-1.0,1.0,1.0, -1.0,1.0,-1.0, 0.0,1.0,
		1.0,-1.0,1.0, 1.0,-1.0,-1.0, 1.0,0.0,
		1.0,1.0,1.0, 1.0,1.0,-1.0, 1.0,1.0
	};


	std::vector<Vertex> verts1;
	std::vector<uint32_t> inds1;
	makeIBO(data, sizeof(data)/sizeof(float), &verts1, &inds1);

	std::vector<Vertex> verts2;
	std::vector<uint32_t> inds2;
	makeIBO(cubeVertices, cubeVertsSize / sizeof(float), &verts2, &inds2);

	std::vector<Vertex> verts3;
	std::vector<uint32_t> inds3;
	makeIBO(data2, sizeof(data2)/sizeof(float), &verts3, &inds3);

	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;


	// create our two meshes to use
	mesh1.loadMeshData(&verts2, &inds2);
	mesh2.loadMeshData(&verts1, &inds1);
	mesh3.loadMeshData(&verts3, &inds3);


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
	Material m4({1.0f,.7,.2}, {.1,.2,.3}, {.5,.6,.2}, 100, 0);

	//materials.push_back(&m1);
	//materials.push_back(&m2);
	//materials.push_back(&m3);
	//materials.push_back(&m4);





	// show axis
	glm::vec3 x = {1,0,0};
	glm::vec3 y = glm::cross(x, glm::vec3(0,0,-1));
	glm::vec3 z = glm::cross(x, y);

	// Create components for each axis
	Component axisX(&mesh1, &m1, x, { 0,0,0 }, { 10,.0125,.0125 });
	Component axisY(&mesh1, &m1, y, { 0,0,0 }, { .0125,10,.0125 });
	Component axisZ(&mesh1, &m1, z, { 0,0,0 }, { .0125,.0125,10 });




	Component p1(&mesh1, &m1, {5,5,5}, { 45,0,60 }, { 2,1,1 });
	Component p2(&mesh2, &m2, { 4,4,4 }, { 0,0,34 }, { 1,2,4 });
	Component p3(&mesh3, &m2, { -4,4,4 }, { -54,0,34 }, { 9,2,4 });


	// This is the model we are trying to build of the components
	Model axes;
	axes.add(&axisX);
	axes.add(&axisY);
	axes.add(&axisZ);

	Model model1;
	model1.add(&p1);


	BoundingBox bb;
	bb.add(&p1);
	bb.add(&p2);
	bb.add(&p3);


	Component a(&mesh1, &m4, glm::vec3(bb.minX, bb.minY, bb.minZ), { 0,0,0 }, { .25,.25,.25 });
	Component b(&mesh1, &m4, glm::vec3(bb.minX, bb.maxY, bb.minZ), { 0,0,0 }, { .25,.25,.25 });
	Component c(&mesh1, &m4, glm::vec3(bb.maxX, bb.maxY, bb.minZ), { 0,0,0 }, { .25,.25,.25 });
	Component d(&mesh1, &m4, glm::vec3(bb.maxX, bb.minY, bb.minZ), { 0,0,0 }, { .25,.25,.25 });
	Component e(&mesh1, &m4, glm::vec3(bb.minX, bb.minY, bb.maxZ), { 0,0,0 }, { .25,.25,.25 });
	Component f(&mesh1, &m4, glm::vec3(bb.minX, bb.maxY, bb.maxZ), { 0,0,0 }, { .25,.25,.25 });
	Component g(&mesh1, &m4, glm::vec3(bb.maxX, bb.maxY, bb.maxZ), { 0,0,0 }, { .25,.25,.25 });
	Component h(&mesh1, &m4, glm::vec3(bb.maxX, bb.minY, bb.maxZ), { 0,0,0 }, { .25,.25,.25 });




	model1.add(&a);
	model1.add(&b);
	model1.add(&c);
	model1.add(&d);
	model1.add(&e);
	model1.add(&f);
	model1.add(&g);
	model1.add(&h);

	model1.add(&p1);

	
	//exit(1);








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


	BatchedMesh* bm = genBatchedMesh({&mesh1, &mesh2, &mesh3});


	std::vector<GLsizei> count_;
	count_.push_back(mesh1.inds->size());
	
	std::vector<GLvoid*> start_;
	start_.push_back((GLvoid*)(*(bm->indexMeshMap))[&mesh1]);

	std::vector<glm::mat4> models__;
	models__.push_back(glm::translate(glm::mat4(1), glm::vec3(0, 10, 0)));







	/*
	* Here, we are putting each material's components information into vectors
	* so they can be drawn using multidraw. For each component, use the indexMeshMap map to
	* find the start index, get the number of indices to draw and get the model, and add to the 
	* vector. Then we can draw each material easily
	*/


	for (Component* c : m1.components) {
		m1.starts.push_back((GLvoid*)(*(bm->indexMeshMap))[c->mesh]);
		m1.counts.push_back(c->mesh->inds->size());
		m1.models.push_back(c->model);
	}

	for (Component* c : m2.components) {
		m2.starts.push_back((GLvoid*)(*(bm->indexMeshMap))[c->mesh]);

		m2.counts.push_back(c->mesh->inds->size());
		m2.models.push_back(c->model);
	}

	for (Component* c : m3.components) {
		m3.starts.push_back((GLvoid*)(*(bm->indexMeshMap))[c->mesh]);

		m3.counts.push_back(c->mesh->inds->size());
		m3.models.push_back(c->model);
	}

	for (Component* c : m4.components) {
		m4.starts.push_back((GLvoid*)(*(bm->indexMeshMap))[c->mesh]);

		m4.counts.push_back(c->mesh->inds->size());
		m4.models.push_back(c->model);
	}








	// Make model matrices and send them over
	unsigned int models;
	models = glGetUniformLocation(program, "models");
	//glUniformMatrix4fv(models, models_.size(), GL_FALSE, glm::value_ptr(models_[0]));



	float cntr = 0.0f;


	// Render previous frame first so the gpu can draw while we do the physics calculations and stuff

	while (!glfwWindowShouldClose(window)) {
		start = std::chrono::high_resolution_clock::now();

		// clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// this must be done each time
		glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));

		// bind the two buffers for drawing
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bm->batchIBO);
		glBindBuffer(GL_ARRAY_BUFFER, bm->batchVBO);

		// give position, normal and texture coordinate data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) * 1));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) * 2));


		/*
		* 
		* Here we draw with the models defined above for each material and not manually adding start/count values
		* 
		*/

		// for every material, draw everything 
		for (Material* m : materials) {
			// skip if empty
			if (m->components.size() == 0) {
				continue;
			}

			m->bindAttributes();
			glUniformMatrix4fv(models, m->models.size(), GL_FALSE, glm::value_ptr(m->models[0]));
			glMultiDrawElements(GL_TRIANGLES, &(m->counts)[0], GL_UNSIGNED_INT, &(m->starts)[0], m->counts.size());

		}

		/*
		* 
		* Physics happens here
		* 
		*/

		// Now do physics and stuff

		p1.rotate.z += .25;
		p1.rotate.x -= .35;
		p1.updateModelMatrix();

		p2.rotate.x += .3;
		p2.updateModelMatrix();

		//std::cout << p1.rotate << "\n";

		bb.update();

		//std::cout << bb.minX << " " << bb.minY << " " << bb.minZ << "\n";



		// update bounding box cube translate values
		a.translate = glm::vec3(bb.minX, bb.minY, bb.minZ);
		b.translate = glm::vec3(bb.minX, bb.maxY, bb.minZ);
		c.translate = glm::vec3(bb.maxX, bb.maxY, bb.minZ);
		d.translate = glm::vec3(bb.maxX, bb.minY, bb.minZ);
		e.translate = glm::vec3(bb.minX, bb.minY, bb.maxZ);
		f.translate = glm::vec3(bb.minX, bb.maxY, bb.maxZ);
		g.translate = glm::vec3(bb.maxX, bb.maxY, bb.maxZ);
		h.translate = glm::vec3(bb.maxX, bb.minY, bb.maxZ);

		// now update the model matricies
		a.updateModelMatrix();
		b.updateModelMatrix();
		c.updateModelMatrix();
		d.updateModelMatrix();
		e.updateModelMatrix();
		f.updateModelMatrix();
		g.updateModelMatrix();
		h.updateModelMatrix();

		
		/*
		* 
		* Update each material's models assuming they have been updated
		* 
		*/

		for (Material* m : materials) {
			m->models.clear();
			for (Component* c : m->components) {
				m->models.push_back(c->model);
			}

		}



		postRenderingSteps(false, window, &start, &proj, &view, width, height);
	}

	cleanup();

}