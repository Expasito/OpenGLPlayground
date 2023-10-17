#include "Mesh.h"


void Mesh::init(int sizeVBO, int sizeIBO) {
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sizeVBO, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ibo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * sizeIBO, NULL, GL_DYNAMIC_DRAW);

}

Mesh::Mesh() {

	// set some default sizes
	verticesBufferSize = 100;
	indicesBufferSize = 200;

	init(verticesBufferSize, indicesBufferSize);
	
}

Mesh::Mesh(int sizeVBO, int sizeIBO) {

	// set the initial sizes
	verticesBufferSize = sizeVBO;
	indicesBufferSize = sizeIBO;

	init(verticesBufferSize, indicesBufferSize);
}


void Mesh::loadMeshData(float* vertices, uint32_t verticesSize, uint32_t* indices, uint32_t indicesSize) {

	// For each buffer, check if we need to resize or not
	// then send the data to the GPU
	
	// handle vertices first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (verticesSize > verticesBufferSize) {
		verticesBufferSize = verticesSize;
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize, vertices, GL_DYNAMIC_DRAW);
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * verticesSize, vertices);
	}

	// now for indices

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	if (indicesSize > indicesBufferSize) {
		indicesBufferSize = indicesSize;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indicesSize, indices, GL_DYNAMIC_DRAW);
	}
	else {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * indicesSize, indices);
	}
}


void Mesh::loadMeshData(std::vector<glm::vec3>* vertices, std::vector<uint32_t>* indices) {

	// For each buffer, check if we need to resize or not
	// then send the data to the GPU
	// 
	// handle vertices first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (vertices->size() > verticesBufferSize) {
		verticesBufferSize = vertices->size();
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices->size(), glm::value_ptr((*vertices)[0]), GL_DYNAMIC_DRAW);
		std::cout << "Resize\n";
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * vertices->size(), glm::value_ptr((*vertices)[0]));

	}

	// now for indices

	// Need to finish implementing

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	if (indices->size() > indicesBufferSize) {
		indicesBufferSize = indices->size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices->size(), &(*indices)[0], GL_DYNAMIC_DRAW);
		std::cout << "Resize\n";
	}
	else {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * indices->size(), &(*indices)[0]);
	}
}