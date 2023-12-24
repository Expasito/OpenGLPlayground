#pragma once
#include "Includes.h"
#include "Component.h"

class Component;
class Material
{
public:
	glm::vec3 albedo;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	// 1 means that we use texture index from an array, 0 means we just use the actual values
	int areTextures;

	std::vector<Component*> components;

	Material(glm::vec3 albedo, glm::vec3 diffuse, glm::vec3 specular, float shininess, float areTextures);

	void bindAttributes();

	void add(Component* c);

	void drawAll();

	void drawBatched();
};

