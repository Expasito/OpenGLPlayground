#include "Component.h"

Component::Component(Mesh* mesh, Material* material, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scalate) {
	this->mesh = mesh;
	this->material = material;
	this->translate = translate;
	this->rotate = rotate;
	this->scalate = scalate;
	updateModelMatrix();
	material->add(this);
};

void Component::updateModelMatrix() {
	glm::mat4 trans(1.0f);
	model = glm::scale(trans, scalate) *
		glm::rotate(trans, glm::radians(rotate.x), { 1,0,0 }) *
		glm::rotate(trans, glm::radians(rotate.y), { 0,1,0 }) *
		glm::rotate(trans, glm::radians(rotate.z), { 0,0,1 }) *
		glm::translate(trans, translate);
};

void Component::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->ibo);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	material->bindAttributes();
	glDrawElements(GL_TRIANGLES, mesh->indicesBufferSize, GL_UNSIGNED_INT, 0);
};