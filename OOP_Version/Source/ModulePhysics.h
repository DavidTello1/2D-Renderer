#pragma once
#include "Module.h"

#include <vector>
#include "glm/include/glm/glm.hpp"

class ComponentRectCollider;
class ComponentCircleCollider;

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

	//bool CheckCollision(ComponentRectCollider* collider1, ComponentRectCollider* collider2);	 // Rect - Rect
	//bool CheckCollision(ComponentCircleCollider* collider1, ComponentCircleCollider* collider2); // Circle - Circle
	//bool CheckCollision(ComponentCircleCollider* collider1, ComponentRectCollider* collider2);	 // Circle - Rect

private:
	std::vector<ComponentRectCollider*> rect_colliders;
	std::vector<ComponentCircleCollider*> circle_colliders;
};

