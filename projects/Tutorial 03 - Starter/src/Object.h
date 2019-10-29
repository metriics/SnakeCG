#pragma once
#include <GLM/glm.hpp>
#include <vector>
#include "Mesh.h"

class Object {
public:
	Object();
	Object(glm::vec3 pos, glm::vec4 col);

	void updateMesh();

	glm::vec3 getPosition();
	void setPosition(glm::vec3);
	void setPosition(float x, float y);

	Mesh::Sptr& getMesh();

	glm::vec4 getColour();

	Vertex* positions = new Vertex[4];

private:
	Mesh::Sptr mesh;
	glm::vec3 position;
	glm::vec4 colour;
};