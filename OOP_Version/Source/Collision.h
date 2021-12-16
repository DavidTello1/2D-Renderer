#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

struct Collision {
	bool has_collided;
	glm::vec2 distance;
	Component::Type type;
	float other_radius; //used for fixing overlaping in circle-circle collisions

	void Reset() {
		has_collided = false;
		distance = glm::vec2(0.0f);
		type = Component::Type::UNKNOWN;
		other_radius = 0.0f;
	}
};
