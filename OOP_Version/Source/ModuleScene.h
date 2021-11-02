#pragma once
#include "Module.h"

#include <vector>
#include "glm/include/glm/glm.hpp"

#define WORLD_WIDTH  1024
#define WORLD_HEIGHT 768

class Entity;
class ComponentCamera;

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

	// --- Demo
	const bool& IsDebug() const { return is_debug; }
	void SwitchDebug() { is_debug = !is_debug; }

	const int& GetWorldWidth() const { return world_width; }
	const int& GetWorldHeight() const { return world_height; }
	void SetWorldWidth(int width) { world_width = width; }
	void SetWorldHeight(int height) { world_height = height; }
	void DrawGrid(float grid_size);

	void AddAsteroids(int num);
	void DeleteAsteroids(int num);

private:
	Entity* CreateEntity();
	void DeleteEntity(Entity* entity);

public:
	ComponentCamera* main_camera = nullptr;
	std::vector<Entity*> entities;

	// --- Demo
private:
	bool is_debug = false;
	float grid_size = 5.0f;

	int world_width, world_height = 0;
};
