#pragma once
#include "Object.h"
#include <GLM/glm.hpp>

class Collision
{
public:
	Collision();
	bool isColliding(Object* A, Object* B); // return true if two provided objects are colliding
};

