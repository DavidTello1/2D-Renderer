#include "ModuleScene.h"

#include "Application.h"
#include "ModuleResources.h"

#include "Entity.h"
#include "ComponentSprite.h"
#include "ComponentCollider.h"
#include "ComponentAsteroid.h"
#include "ComponentAnimation.h"

ModuleScene::ModuleScene(bool start_enabled) : Module("ModuleScene", start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	// Create Main Camera
	Entity* cam = CreateEntity();
	main_camera = (ComponentCamera*)cam->AddComponent(Component::Type::CAMERA);

	return true;
}

bool ModuleScene::Start()
{
	Entity* entity = CreateEntity();
	entity->AddComponent(Component::Type::TRANSFORM);
	entity->AddComponent(Component::Type::RENDERER);
	ComponentSprite* sprite = (ComponentSprite*)entity->AddComponent(Component::Type::SPRITE);

	Texture* tex = App->resources->LoadTexture("Assets/asteroids.png");
	sprite->SetTexture(tex->index);
	sprite->SetSize(glm::vec2(100.0f));

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
	entities.push_back(entity);
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

//--------------------------------------
void ModuleScene::AddAsteroids(int num)
{
	for (int i = 0; i < num; ++i)
	{
		Entity* entity = CreateEntity();
		entity->AddComponent(Component::Type::TRANSFORM);
		entity->AddComponent(Component::Type::RENDERER);

		ComponentSprite* sprite = (ComponentSprite*)entity->AddComponent(Component::Type::SPRITE);
		sprite->SetTexture(App->resources->LoadTexture("Assets/asteroids.png")->index);
		sprite->SetSize(glm::vec2(100.0f));

		ComponentCollider* collider = (ComponentCollider*)entity->AddComponent(Component::Type::COLLIDER);
		//---

		ComponentAsteroid* asteroid = (ComponentAsteroid*)entity->AddComponent(Component::Type::ASTEROID);
		//---

		ComponentAnimation* animation = (ComponentAnimation*)entity->AddComponent(Component::Type::ANIMATION);
		//---
	}
}

void ModuleScene::DeleteAsteroids(int num)
{
	for (int i = 0; i < num; ++i)
		DeleteEntity(entities.front());
}
