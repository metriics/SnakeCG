#pragma once
#include <GLM/glm.hpp>
#include <vector>
#include "Mesh.h"

class Object {
public:
	Object();
	Object(glm::vec3 pos, glm::vec4 col, int dir);

	void updateMesh();

	glm::vec3 getPosition();
	void setPosition(glm::vec3);
	void addToPosition(float x, float y, float z);

	Mesh::Sptr& getMesh();

	int getDirection();
	void setDirection(int dir);

	glm::vec4 getColour();

	Vertex* positions = new Vertex[4];

private:
	Mesh::Sptr mesh;
	glm::vec3 position;
	glm::vec4 colour;
	int direction;
};