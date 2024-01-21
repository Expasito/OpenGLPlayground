#pragma once
#include "Includes.h"
#include "Material.h"

class Material;
class Mesh;

class Component
{

public:
	Mesh* mesh;
	Material* material;
	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scalate;
	glm::mat4 model;

	Component(Mesh* mesh, Material* material, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scalate, void(*init)(Component*), void(*loop)(Component*));

	void updateModelMatrix();

	void draw();


	struct ComponentMemory {
		float* floats;
		int numFloats;
		int* ints;
		int numInts;
	};

	// members for init, loop and memory
	ComponentMemory* memory;

	// These are function pointers to the init and loop functions
	void (*init)(Component*);
	void (*loop)(Component*);

private:

};

