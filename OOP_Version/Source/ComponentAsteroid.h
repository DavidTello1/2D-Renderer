#pragma once
#include "Component.h"

class ComponentAsteroid : public Component
{
public:
	ComponentAsteroid(Entity* entity) : Component(Component::Type::ASTEROID, entity) {};
	~ComponentAsteroid() {};

	static inline Component::Type GetType() { return Component::Type::ASTEROID; }


public:
	//glm::vec2 direction;
	//float speed;
};

