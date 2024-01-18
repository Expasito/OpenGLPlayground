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

		if (areTextures == 1) {
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, textureMap[(int)albedo.x]);
		}
		
};

void Material::add(Component* c) {
	components.push_back(c);
	//counts.push_back(c->mesh->inds->size());
	//models.push_back(c->model);

	// get the index based on the meshmap provided
	// But not for now
	//starts.push_back(
	//	(GLvoid *) ((*indexMeshMap)[c->mesh])
	//);
}

void Material::drawAll() {
	bindAttributes();
	for (Component* c : components) {
		c->draw();
	}
}

void Material::drawBatched() {
	bindAttributes();

	
}

void Material::setMeshMap(std::map<Mesh*, uint32_t>* meshMap) {
	indexMeshMap = meshMap;
}