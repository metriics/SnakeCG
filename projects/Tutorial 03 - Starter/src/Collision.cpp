#include "Collision.h"

Collision::Collision()
{
}

bool Collision::isColliding(Object* A, Object* B)
{
	glm::vec3 AP = A->getPosition();
	glm::vec3 BP = B->getPosition();
	glm::vec3 temp = AP - BP;
	
	if (glm::length(temp) < 0.025) {
		return true;
	}
	else {
		return false;
	}
}
