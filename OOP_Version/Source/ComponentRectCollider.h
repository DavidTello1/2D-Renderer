#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

class Entity;
class ComponentCircleCollider;

class ComponentRectCollider : public Component
{
public:
	ComponentRectCollider(Entity* entity) : position(0.0f), size(0.0f), offset(0.0f), is_colliding(false), Component(Component::Type::RECT_COLLIDER, entity) {};
	~ComponentRectCollider() {};

	static inline Component::Type GetType() { return Component::Type::RECT_COLLIDER; }

	void OnUpdate(float dt) override;
	
	const bool& IsColliding() const { return is_colliding; }
	const glm::vec2& GetPosition() const { return position; }
	const glm::vec2& GetSize() const { return size; }
	const glm::vec2& GetOffset() const { return offset; }

	void SetSize(glm::vec2 size_) { size = size_; }
	void SetOffset(glm::vec2 offset_) { offset = offset_; }

	bool CheckCollision(ComponentCircleCollider* collider);
	bool CheckCollision(ComponentRectCollider* collider);

private:
	void DoCollisions();

private:
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(0.0f);

	glm::vec2 offset = glm::vec2(0.0f);

	bool is_colliding = false;
};