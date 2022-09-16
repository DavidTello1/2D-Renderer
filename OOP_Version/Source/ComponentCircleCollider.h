#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

class ComponentRectCollider;

class ComponentCircleCollider : public Component
{
public:
	ComponentCircleCollider(Entity* entity);
	~ComponentCircleCollider();

	void OnUpdate(float dt) override;

	static inline Component::Type GetType() { return Component::Type::CIRCLE_COLLIDER; }

	const bool& IsColliding() const { return is_colliding; }
	const bool& IsStatic() const { return is_static; }
	glm::vec2 GetPosition();

	const glm::vec2& GetCenter() const { return center; }
	const float& GetRadius() const { return radius; }
	const glm::vec2& GetOffset() const { return offset; }

	void SetStatic(bool value) { is_static = value; }
	void SetColliding(bool value) { is_colliding = value; }
	void SetCenter(glm::vec2 new_center) { center = new_center; }
	void SetRadius(float radius_) { radius = radius_; }
	void SetOffset(glm::vec2 offset_) { offset = offset_; }

private:
	bool is_colliding = false;
	bool is_static = false;

	glm::vec2 center = glm::vec2(0.0f);
	float radius = 0.0f;
	glm::vec2 offset = glm::vec2(0.0f);
};
