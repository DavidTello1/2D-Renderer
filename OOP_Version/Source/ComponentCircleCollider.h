#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

class Entity;
class ComponentRectCollider;

class ComponentCircleCollider : public Component
{
public:
	ComponentCircleCollider(Entity* entity) : center(0.0f), radius(0.0f), offset(0.0f), is_colliding(false), Component(Component::Type::CIRCLE_COLLIDER, entity) {};
	~ComponentCircleCollider() {};

	void OnUpdate(float dt) override;

	static inline Component::Type GetType() { return Component::Type::CIRCLE_COLLIDER; }

	const bool& IsColliding() const { return is_colliding; }
	glm::vec2 GetPosition();

	const glm::vec2& GetCenter() const { return center; }
	const float& GetRadius() const { return radius; }
	const glm::vec2& GetOffset() const { return offset; }

	void SetCenter(glm::vec2 new_center) { center = new_center; }
	void SetRadius(float radius_) { radius = radius_; }
	void SetOffset(glm::vec2 offset_) { offset = offset_; }

	bool CheckCollision(ComponentCircleCollider* collider);
	bool CheckCollision(ComponentRectCollider* collider);

private:
	void DoCollisions();

public:
	glm::vec2 dist = glm::vec2(0.0f);
	bool isBoundariesCollision = false;
	bool isStatic = false;

	float radius_collision = 0.0f;

private:
	glm::vec2 center = glm::vec2(0.0f);
	float radius = 0.0f;

	glm::vec2 offset = glm::vec2(0.0f);

	bool is_colliding = false;
};
