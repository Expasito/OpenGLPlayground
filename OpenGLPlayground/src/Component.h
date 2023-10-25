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

	Component(Mesh* mesh, Material* material, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scalate);

	void updateModelMatrix();

	void draw();

private:

};

