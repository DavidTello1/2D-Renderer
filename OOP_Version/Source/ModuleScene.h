#pragma once
#include "Module.h"

#include <vector>
#include "glm/include/glm/glm.hpp"

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
};
