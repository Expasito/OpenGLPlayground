#include "Material.h"


Material::Material(glm::vec3 albedo, glm::vec3 diffuse, glm::vec3 specular, float shininess, float areTextures) {
	this->albedo = albedo;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
	this->areTextures = areTextures;
};

void Material::bindAttributes() {
		glUniform3fv(matAlbedo, 1, glm::value_ptr(albedo));
		glUniform3fv(matDiffuse, 1, glm::value_ptr(diffuse));
		glUniform3fv(matSpecular, 1, glm::value_ptr(specular));
		glUniform1f(matShininess, shininess);
		glUniform1i(matAreTextures, areTextures);
};

void Material::add(Component* c) {
	components.push_back(c);
}

void Material::drawAll() {
	bindAttributes();
	for (Component* c : components) {
		c->draw();
	}
}