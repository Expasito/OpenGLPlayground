#pragma once
#include "../src/Includes.h"
#include "../utils/Utils.h"

struct Vertex;

#include <vector>
/*
* 
* Mesh will hold the mesh data for a chunk
* 
*/
class Mesh
{
public:

	// OpenGL buffer numbers
	unsigned int vbo;
	unsigned int ibo;

	std::vector<Vertex>* verts;
	std::vector<uint32_t>* inds;

	// what is the max capacity of the buffer
	uint32_t verticesMaxCapacity;
	uint32_t indicesMaxCapacity;

	// what is the current capacity
	uint32_t verticesCurSize;
	uint32_t indicesCurSize;


	Mesh();

	Mesh(int verticesSize, int indicesSize);

	void loadMeshData(float* vertices, uint32_t verticesSize, uint32_t* indices, uint32_t indicesSize);

	void loadMeshData(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices);

	void appendData(std::vector <Vertex>* vertices, std::vector<uint32_t>* indices);


private:
	void init(int sizeVBO, int sizeIBO);


};

