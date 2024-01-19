#pragma once
#include "Includes.h"
#include "Component.h"
#include "../utils/Mesh.h"

class Component;
class Mesh;
class Material
{
public:
	glm::vec3 albedo;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	// 1 means that we use texture index from an array, 0 means we just use the actual values
	int areTextures;

	bool updateModelFlag = false;

	std::vector<Component*> components;

	std::vector<glm::mat4> models;
	std::vector<GLsizei> counts;
	std::vector<GLvoid*> starts;

	Material(glm::vec3 albedo, glm::vec3 diffuse, glm::vec3 specular, float shininess, float areTextures);

	void bindAttributes();

	void add(Component* c);

	void drawAll();

	void drawBatched();

};

