#pragma once
#include "Module.h"

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	virtual ~ModuleScene();

	bool Init() override;

public:
	//Camera* camera = nullptr;
	//std::vector<Entity*> entities;
};
