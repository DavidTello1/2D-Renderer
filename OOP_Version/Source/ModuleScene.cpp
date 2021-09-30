#include "ModuleScene.h"
#include "ModuleResources.h"

ModuleScene* M_Scene = nullptr;

ModuleScene::ModuleScene(bool start_enabled) : Module("ModuleScene", start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	return true;
}
