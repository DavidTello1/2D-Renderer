#pragma once
#include "Collision.h"

class ComponentCircleCollider;

class ComponentRectCollider : public Component
{
public:
	ComponentRectCollider(Entity* entity);
	~ComponentRectCollider();

	static inline Component::Type GetType() { return Component::Type::RECT_COLLIDER; }

	void OnUpdate(float dt) override;
	
	const bool& IsColliding() const { return collision.has_collided; }
	const bool& IsStatic() const { return is_static; }
	const Collision& GetCollision() const { return collision; }

	const glm::vec2& GetPosition() const { return position; }
	const glm::vec2& GetSize() const { return size; }
	const glm::vec2& GetOffset() const { return offset; }

	void SetStatic(bool value) { is_static = value; }
	void SetCollision(Collision col) { collision = col; }

	void SetSize(glm::vec2 size_) { size = size_; }
	void SetOffset(glm::vec2 offset_) { offset = offset_; }

	bool CheckCollision(ComponentCircleCollider* collider);
	bool CheckCollision(ComponentRectCollider* collider);
	void DoCollisions();

private:
	bool is_static = false;
	Collision collision;

	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(0.0f);
	glm::vec2 offset = glm::vec2(0.0f);
};