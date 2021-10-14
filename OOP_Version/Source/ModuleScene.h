#pragma once
#include "Module.h"

#include <vector>

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

	void AddAsteroids(int num);
	void DeleteAsteroids(int num);

private:
	Entity* CreateEntity();
	void DeleteEntity(Entity* entity);

public:
	ComponentCamera* main_camera = nullptr;
	std::vector<Entity*> entities;
};
