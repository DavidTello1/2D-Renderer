#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

class ComponentTransform;
class ComponentCircleCollider;

class ComponentAsteroid : public Component
{
public:
	ComponentAsteroid(Entity* entity) : direction(0.0f), velocity(0.0f), Component(Component::Type::ASTEROID, entity) {};
	~ComponentAsteroid() {};

	static inline Component::Type GetType() { return Component::Type::ASTEROID; }

	void OnUpdate(float dt) override;

	const glm::vec2& GetDirection() const { return direction; }
	const glm::vec2& GetVelocity() const { return velocity; }

	void SetDirection(glm::vec2 dir) { direction = dir; }
	void SetVelocity(glm::vec2 vel) { velocity = vel; }

	void SetRandomValues();

	void OnCollision(ComponentCircleCollider* collider, ComponentTransform* transform);
	void OnCollision2(ComponentCircleCollider* collider, ComponentTransform* transform);

public:
	glm::vec2 direction = glm::vec2(0.0f);
	glm::vec2 velocity = glm::vec2(0.0f);
};

