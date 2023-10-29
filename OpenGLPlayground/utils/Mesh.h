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

	unsigned int vbo;
	unsigned int ibo;

	float* vertices;
	unsigned int* indices;

	uint32_t verticesBufferSize;
	uint32_t indicesBufferSize;


	Mesh();

	Mesh(int verticesSize, int indicesSize);

	void loadMeshData(float* vertices, uint32_t verticesSize, uint32_t* indices, uint32_t indicesSize);

	void loadMeshData(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices);




private:
	void init(int sizeVBO, int sizeIBO);


};

