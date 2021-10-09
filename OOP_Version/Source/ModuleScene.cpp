#include "ModuleScene.h"

#include "Application.h"
#include "ModuleResources.h"

#include "Entity.h"

ModuleScene::ModuleScene(bool start_enabled) : Module("ModuleScene", start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	// Create Main Camera


	return true;
}

bool ModuleScene::Update(float dt)
{

	return true;
}

bool ModuleScene::CleanUp()
{
	for (Entity* entity : entities)
		DeleteEntity(entity);
	entities.clear();

	return true;
}

void ModuleScene::Draw()
{
	GLuint shader = App->resources->default_shader;
	glUseProgram(shader);

	for (Entity* entity : entities)
		entity->Draw(shader);
}

//--------------------------------------
Entity* ModuleScene::CreateEntity()
{
	Entity* entity = new Entity();
	return entity;
}

void ModuleScene::DeleteEntity(Entity* entity)
{
	for (size_t i = 0, size = entities.size(); i < size; ++i)
	{
		if (entities[i] == entity)
		{
			RELEASE(entity);
			entities.erase(entities.begin() + i);
			break;
		}
	}
}
