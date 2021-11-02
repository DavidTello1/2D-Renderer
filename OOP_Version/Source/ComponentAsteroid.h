#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

class ComponentAsteroid : public Component
{
public:
	ComponentAsteroid(Entity* entity) : direction(0.0f), speed(0.0f), Component(Component::Type::ASTEROID, entity) {};
	~ComponentAsteroid() {};

	static inline Component::Type GetType() { return Component::Type::ASTEROID; }

	void OnUpdate(float dt) override;

	const glm::vec2& GetDirection() const { return direction; }
	const float& GetSpeed() const { return speed; }

	void SetDirection(glm::vec2 dir) { direction = dir; }
	void SetSpeed(float speed_) { speed = speed_; }

	void SetRandomValues();

	void OnCollision();

public:
	glm::vec2 direction = glm::vec2(0.0f);
	float speed = 0.0f;
};

