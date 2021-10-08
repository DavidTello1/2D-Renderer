#pragma once
#include "Module.h"
#include "glm/include/glm/glm.hpp"

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	virtual ~ModuleScene();

	bool Init() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	void Draw();

public:
	//Camera* camera = nullptr;
	//std::vector<Entity*> entities;

private:
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
};
