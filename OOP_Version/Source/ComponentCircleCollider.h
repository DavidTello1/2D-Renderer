#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

class Entity;

class ComponentCircleCollider : public Component
{
public:
	ComponentCircleCollider(Entity* entity) : center(0.0f), radius(0.0f), offset(0.0f), Component(Component::Type::CIRCLE_COLLIDER, entity) {};
	~ComponentCircleCollider() {};

	void OnUpdate(float dt) override;

	static inline Component::Type GetType() { return Component::Type::CIRCLE_COLLIDER; }

	const glm::vec2& GetCenter() const { return center; }
	const float& GetRadius() const { return radius; }
	const glm::vec2& GetOffset() const { return offset; }

	void SetRadius(float radius_) { radius = radius_; }
	void SetOffset(glm::vec2 offset_) { offset = offset_; }

	Entity* GetCollision();
	void ShowCollider(); // Debug Draw

private:
	glm::vec2 center = glm::vec2(0.0f);
	float radius = 0.0f;

	glm::vec2 offset = glm::vec2(0.0f);
};
