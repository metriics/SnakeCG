#include "Collision.h"

Collision::Collision()
{
}

bool Collision::isColliding(Object* A, Object* B)
{
	glm::vec3 AP = A->getPosition(); 
	glm::vec3 BP = B->getPosition();
	glm::vec3 dPos = AP - BP; // get delta pos
	
	if (glm::length(dPos) < 0.025) { // if dPos < size of square, then the objs are colliding and return true
		return true;
	}
	else { 
		return false;
	}
}
