#pragma once
#include "Module.h"
#include "Component.h"

#include <vector>
#include "glm/include/glm/glm.hpp"

class Entity;
class ComponentRectCollider;
class ComponentCircleCollider;

struct Collision {
	bool has_collided = false;
	glm::vec2 distance = glm::vec2(0.0f);
};

class ModulePhysics : public Module
{
public:
	ModulePhysics(bool start_enabled = true) : Module("ModuleScene", start_enabled) {};
	virtual ~ModulePhysics() {};

	bool Init() override;
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	void DrawColliders();

	void AddCollider(ComponentRectCollider* collider);
	void AddCollider(ComponentCircleCollider* collider);
	void RemoveCollider(ComponentRectCollider* collider);
	void RemoveCollider(ComponentCircleCollider* collider);

private:
	int Exists(ComponentRectCollider* collider);
	int Exists(ComponentCircleCollider* collider);

	const int& GetCollisionDirection(glm::vec2 distance) const;

	Collision CheckCollision(ComponentRectCollider* collider1, ComponentRectCollider* collider2);	  // Rect - Rect
	Collision CheckCollision(ComponentRectCollider* collider1, ComponentCircleCollider* collider2);	  // Rect - Circle
	Collision CheckCollision(ComponentCircleCollider* collider1, ComponentCircleCollider* collider2); // Circle - Circle
	Collision CheckCollision(ComponentCircleCollider* collider1, ComponentRectCollider* collider2);	  // Circle - Rect

	void ResolveCollision(glm::vec2 distance, ComponentRectCollider* collider1, ComponentRectCollider* collider2);		  // Rect - Rect
	void ResolveCollision(glm::vec2 distance, ComponentRectCollider* collider1, ComponentCircleCollider* collider2);	  // Rect - Circle
	void ResolveCollision(glm::vec2 distance, ComponentCircleCollider* collider1, ComponentCircleCollider* collider2);	  // Circle - Circle
	void ResolveCollision(glm::vec2 distance, ComponentCircleCollider* collider1, ComponentRectCollider* collider2);	  // Circle - Rect

private:
	std::vector<ComponentRectCollider*> rect_colliders;
	std::vector<ComponentCircleCollider*> circle_colliders;
};

