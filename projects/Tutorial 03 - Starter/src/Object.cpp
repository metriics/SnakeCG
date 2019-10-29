#include "Object.h"

Object::Object()
{
}

Object::Object(glm::vec3 pos, glm::vec4 col) {
	this->position = pos;
	this->colour = col;

	//positions = { {position.x, position.y, position.z }, {colour.x, colour.y, colour.z, colour.w} };
	
	
	
	positions[0].Position = this->position + glm::vec3(-0.025, 0.025, 0); // bl
	positions[1].Position = this->position + glm::vec3(0.025, 0.025, 0); // br
	positions[2].Position = this->position + glm::vec3(-0.025, -0.025, 0); // tl
	positions[3].Position = this->position + glm::vec3(0.025, -0.025, 0); // tr

	for (int i = 0; i < 4; i++) {
		positions[i].Color = col;
	}
	updateMesh();
}

void Object::updateMesh()
{
	// Create our 6 indices
	uint32_t indices[6] = {
		0, 1, 2,
		2, 1, 3
	};

	mesh = std::make_shared<Mesh>(positions, 4, indices, 6);
}

glm::vec3 Object::getPosition()
{
	return glm::vec3();
}

void Object::setPosition(glm::vec3 pos) {
	this->position = pos;

	positions[0].Position = this->position + glm::vec3(-0.025, 0.025, 0); // bl
	positions[1].Position = this->position + glm::vec3(0.025, 0.025, 0); // br
	positions[2].Position = this->position + glm::vec3(-0.025, -0.025, 0); // tl
	positions[3].Position = this->position + glm::vec3(0.025, -0.025, 0); // tr

	updateMesh();
}

void Object::setPosition(float x, float y)
{
	this->position.x += x;
	this->position.y += y;

	setPosition(position);
}

Mesh::Sptr& Object::getMesh()
{
	return mesh;
}

glm::vec4 Object::getColour()
{
	return this->colour;
}
