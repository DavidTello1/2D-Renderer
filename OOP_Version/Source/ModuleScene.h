#pragma once
#include "Module.h"

#include <vector>

#define BASE_ENTITIES 6 //camera + background + boundaries(4)
#define BOUNDARIES_SIZE 100
#define DEFAULT_WORLD_WIDTH 2500
#define DEFAULT_WORLD_HEIGHT 2500

class Entity;
class ComponentCameraController;
class ComponentTransform;

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	virtual ~ModuleScene();

	bool Init() override;
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	void Draw();

	const std::vector<Entity*>& GetEntities() const { return entities; }

	// --- Demo ---
	// World
	const int& GetWorldWidth() const { return world_width; }
	const int& GetWorldHeight() const { return world_height; }

	void SetWorldWidth(int width) { world_width = width; UpdateWorldSize(); }
	void SetWorldHeight(int height) { world_height = height; UpdateWorldSize(); }

	void UpdateWorldSize();

	// Asteroids
	void AddAsteroids(int num);
	void DeleteAsteroids(int num);

private:
	Entity* CreateEntity();
	void DeleteEntity(Entity* entity);

private:
	std::vector<Entity*> entities;

	// --- Demo
	int world_width = 0;
	int world_height = 0;

	ComponentTransform* background = nullptr;
	Entity* b_top = nullptr;
	Entity* b_bottom = nullptr;
	Entity* b_left = nullptr;
	Entity* b_right = nullptr;
};
