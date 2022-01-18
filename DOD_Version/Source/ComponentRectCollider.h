#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

class ComponentCircleCollider;

class ComponentRectCollider : public Component
{
public:
	ComponentRectCollider(Entity* entity);
	~ComponentRectCollider();

	static inline Component::Type GetType() { return Component::Type::RECT_COLLIDER; }

	void OnUpdate(float dt) override;
	
	const bool& IsColliding() const { return is_colliding; }
	const bool& IsStatic() const { return is_static; }

	const glm::vec2& GetPosition() const { return position; }
	const glm::vec2& GetSize() const { return size; }
	const glm::vec2& GetOffset() const { return offset; }

	void SetStatic(bool value) { is_static = value; }
	void SetColliding(bool value) { is_colliding = value; }
	void SetSize(glm::vec2 size_) { size = size_; }
	void SetOffset(glm::vec2 offset_) { offset = offset_; }

private:
	bool is_colliding = false;
	bool is_static = false;

	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(0.0f);
	glm::vec2 offset = glm::vec2(0.0f);
};