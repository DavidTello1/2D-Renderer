#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

class Entity;

class ComponentCollider : public Component
{
public:
	ComponentCollider(Entity* entity, glm::vec2 pos = glm::vec2(0.0f), glm::vec2 size_ = glm::vec2(0.0f), float radius_ = 0.0f) 
		: position(pos), size(size_), radius(radius_), Component(Component::Type::COLLIDER, entity) {};
	~ComponentCollider() {};

	static inline Component::Type GetType() { return Component::Type::COLLIDER; }

	void UpdateCollider();
	Entity* GetCollision();

private:
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(0.0f);
	float radius = 0.0f;
};

