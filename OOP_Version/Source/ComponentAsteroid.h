#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

#define MAX_VELOCITY 150
#define MIN_VELOCITY 50
#define MIN_ROTATION_SPEED 5
#define MAX_ROTATION_SPEED 120

class ComponentTransform;
class ComponentCircleCollider;

class ComponentAsteroid : public Component
{
public:
	ComponentAsteroid(Entity* entity) : mass(1.0f), velocity(0.0f), rotation_speed(0.0f), Component(Component::Type::ASTEROID, entity) {};
	~ComponentAsteroid() {};

	static inline Component::Type GetType() { return Component::Type::ASTEROID; }

	void Move(float dt);

	const glm::vec2& GetVelocity() const { return velocity; }
	const float& GetMass() const { return mass; }
	const float& GetRotationSpeed() const { return rotation_speed; }

	void SetVelocity(glm::vec2 vel) { velocity = vel; }
	void SetMass(float mass_) { mass = mass_; }
	void SetRotationSpeed(float speed) { rotation_speed = speed; }

	void SetRandomValues();

private:
	glm::vec2 velocity = glm::vec2(0.0f);
	float mass = 1.0f;
	float rotation_speed = 0.0f;
};

