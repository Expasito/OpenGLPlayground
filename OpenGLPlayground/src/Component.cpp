#include "Component.h"

Component::Component(Mesh* mesh, Material* material, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scalate) {
	this->mesh = mesh;
	this->material = material;
	this->translate = translate;
	this->rotate = rotate;
	this->scalate = scalate;
	updateModelMatrix();

	// add this model to the material so it knows to draw this too
	material->add(this);
};

void Component::updateModelMatrix() {
	glm::mat4 trans(1.0f);
	model = glm::translate(trans, translate) * 
		glm::rotate(trans, glm::radians(rotate.x), { 1,0,0 }) *
		glm::rotate(trans, glm::radians(rotate.y), { 0,1,0 }) *
		glm::rotate(trans, glm::radians(rotate.z), { 0,0,1 }) *
		glm::scale(trans, scalate);

};

void Component::draw() {

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	// give position, normal and texture coordinate data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) * 1));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) * 2));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	material->bindAttributes();
	glDrawElements(GL_TRIANGLES, mesh->indicesMaxCapacity, GL_UNSIGNED_INT, 0);
};