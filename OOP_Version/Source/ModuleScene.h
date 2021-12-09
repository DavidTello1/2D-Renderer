#pragma once
#include "Module.h"

#include <vector>

#define BASE_ENTITIES 6 //camera + background + boundaries(4)
#define WORLD_WIDTH 500
#define WORLD_HEIGHT 500
#define BOUNDARIES_SIZE 100

class Entity;
class ComponentCameraController;
class ComponentSprite;

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
	void DrawDebug();

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

	ComponentSprite* background = nullptr;
	Entity* b_top = nullptr;
	Entity* b_bottom = nullptr;
	Entity* b_left = nullptr;
	Entity* b_right = nullptr;
};
