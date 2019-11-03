#pragma once
#include <GLM/glm.hpp>
#include <vector>
#include "Mesh.h"

class Object {
public:
	Object();
	Object(glm::vec3 pos, glm::vec4 col, int dir);

	void updateMesh(); // calculate new vert positions based on current obj pos. See comment bellow
	// we can only do this because this is a 2d square, and we know its size. This would NOT be practical for a 3d model or any complicated shapes

	glm::vec3 getPosition(); // return objects position
	void setPosition(glm::vec3); // set objects position
	void addToPosition(float x, float y, float z); // add to objects current position

	Mesh::Sptr& getMesh(); // return obbject mesh

	int getDirection(); // get object's direction (used for snake)
	void setDirection(int dir); // set object's direction (used for snake)

	glm::vec4 getColour(); // get object color

	Vertex* positions = new Vertex[4]; // vertex list

private:
	Mesh::Sptr mesh; // mesh shared pointer
	glm::vec3 position; // 3d vector for storing position
	glm::vec4 colour; // 4d vector for storing color (rgba)
	int direction; // current direction
};

class ScoreDot : public Object {
public:
	ScoreDot();
	ScoreDot(glm::vec3 pos); // we have a seperate class with overridden constructor because of hardcoded size values. score dot is smaller
};