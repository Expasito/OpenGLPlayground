#include "Mesh.h"


void Mesh::init(int sizeVBO, int sizeIBO) {
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * sizeVBO, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ibo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * sizeIBO, NULL, GL_DYNAMIC_DRAW);

}

Mesh::Mesh() {

	// set some default sizes
	verticesMaxCapacity = 1;
	indicesMaxCapacity = 1;

	init(verticesMaxCapacity, indicesMaxCapacity);
	
}

Mesh::Mesh(int sizeVBO, int sizeIBO) {

	// set the initial sizes
	verticesMaxCapacity = sizeVBO;
	indicesMaxCapacity = sizeIBO;

	init(verticesMaxCapacity, indicesMaxCapacity);
}


void Mesh::loadMeshData(float* vertices, uint32_t verticesSize, uint32_t* indices, uint32_t indicesSize) {

	// For each buffer, check if we need to resize or not
	// then send the data to the GPU
	
	// handle vertices first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (verticesSize > verticesMaxCapacity) {
		verticesMaxCapacity = verticesSize;
		verticesCurSize = verticesSize;
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize, vertices, GL_DYNAMIC_DRAW);
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * verticesSize, vertices);
	}

	// now for indices

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	if (indicesSize > indicesMaxCapacity) {
		indicesMaxCapacity = indicesSize;
		indicesCurSize = indicesSize;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indicesSize, indices, GL_DYNAMIC_DRAW);
	}
	else {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * indicesSize, indices);
	}
}


void Mesh::loadMeshData(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices) {

	// For each buffer, check if we need to resize or not
	// then send the data to the GPU
	// 
	// handle vertices first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (vertices->size() > verticesMaxCapacity) {
		verticesMaxCapacity = vertices->size();
		//vertices
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices->size(), &(*vertices)[0], GL_DYNAMIC_DRAW);
		std::cout << "Resize\n";
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices->size(), &(*vertices)[0]);

	}

	// now for indices

	// Need to finish implementing

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	if (indices->size() > indicesMaxCapacity) {
		indicesMaxCapacity = indices->size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices->size(), &(*indices)[0], GL_DYNAMIC_DRAW);
		std::cout << "Resize\n";
	}
	else {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * indices->size(), &(*indices)[0]);
	}
}

void Mesh::appendData(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices) {

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	if (vertices->size() + verticesCurSize >= verticesMaxCapacity) {
		unsigned int copy;
		glGenBuffers(1, &copy);
		glBindBuffer(GL_COPY_READ_BUFFER, copy);
		glBufferData(GL_COPY_READ_BUFFER, sizeof(Vertex) * (verticesCurSize + 1), NULL, GL_DYNAMIC_DRAW);
		checkErrors();
		std::cout << "No errors\n";
		exit(1);
		glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER, 0, 0, sizeof(Vertex) * verticesCurSize);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (vertices->size() + verticesCurSize), NULL, GL_DYNAMIC_DRAW);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, sizeof(Vertex) * verticesCurSize);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * verticesCurSize, sizeof(Vertex) * vertices->size(), &(*vertices)[0]);
		glDeleteBuffers(1, &copy);

		verticesCurSize += vertices->size();
		verticesMaxCapacity = verticesCurSize;

	}
	else {
		// just copy the data from the previous cuttof and upate
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * verticesCurSize, sizeof(Vertex) * vertices->size(), &(*vertices)[0]);
		verticesCurSize += vertices->size();
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	if (indices->size() + indicesCurSize >= indicesMaxCapacity) {
		unsigned int copy;
		glGenBuffers(1, &copy);
		glBindBuffer(GL_COPY_READ_BUFFER, copy);
		glBufferData(GL_COPY_READ_BUFFER, sizeof(uint32_t) * indicesMaxCapacity, NULL, GL_DYNAMIC_DRAW);
		glCopyBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GL_COPY_READ_BUFFER, 0, 0, sizeof(uint32_t) * indicesCurSize);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * (indices->size() + indicesCurSize), NULL, GL_DYNAMIC_DRAW);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, sizeof(uint32_t) * indicesCurSize);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indicesCurSize, sizeof(uint32_t) * indices->size(), &(*indices)[0]);
		glDeleteBuffers(1, &copy);

		indicesCurSize += indices->size();
		indicesMaxCapacity = indicesCurSize;
	}
	else {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indicesCurSize, sizeof(uint32_t) * indices->size(), &(*indices)[0]);
		indicesCurSize += indices->size();
	}

}