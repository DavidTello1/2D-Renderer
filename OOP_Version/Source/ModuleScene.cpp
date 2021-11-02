#include "ModuleScene.h"

#include "Application.h"
#include "ModuleResources.h"

#include "Entity.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentSprite.h"
#include "ComponentRectCollider.h"
#include "ComponentCircleCollider.h"
#include "ComponentAsteroid.h"

#include "mmgr/mmgr.h"

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
	entity->AddComponent(Component::Type::RENDERER);

	ComponentTransform* transform = (ComponentTransform*)entity->AddComponent(Component::Type::TRANSFORM);
	transform->SetPosition(glm::vec2(100.0f, 100.0f));

	ComponentSprite* sprite = (ComponentSprite*)entity->AddComponent(Component::Type::SPRITE);
	sprite->SetTexture(App->resources->LoadTexture("Assets/asteroids.png")->index);
	sprite->SetSize(glm::vec2(100.0f));

	return true;
}

bool ModuleScene::Update(float dt)
{
	for (Entity* entity : entities)
	{
		for(Component* component : entity->GetComponents())
			component->OnUpdate(dt);
	}

	if (is_debug)
		DrawGrid(grid_size);

	return true;
}

bool ModuleScene::CleanUp()
{
	while(!entities.empty())
		DeleteEntity(entities.front());
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
void ModuleScene::DrawGrid(float grid_size)
{

}

void ModuleScene::AddAsteroids(int num)
{
	for (int i = 0; i < num; ++i)
	{
		Entity* entity = CreateEntity();
		entity->AddComponent(Component::Type::RENDERER);

		ComponentTransform* transform = (ComponentTransform*)entity->AddComponent(Component::Type::TRANSFORM);
		float pos_x = 20.0f * i;
		float pos_y = 50.0f * i;

		transform->SetPosition(glm::vec2(pos_x, pos_y));
		transform->SetScale(glm::vec2(2.0f));

		ComponentSprite* sprite = (ComponentSprite*)entity->AddComponent(Component::Type::SPRITE);
		sprite->SetTexture(App->resources->LoadTexture("Assets/asteroids.png")->index); //***CHANGE TO RANDOM (1-3)
		sprite->SetSize(glm::vec2(100.0f));
		sprite->SetOffset(glm::vec2(0.0f, 0.0f)); //***CHANGE TO RANDOM (1-3)

		ComponentRectCollider* collider = (ComponentRectCollider*)entity->AddComponent(Component::Type::RECT_COLLIDER); //***CHANGE TO CIRCLE
		collider->SetSize(sprite->GetSize() * transform->GetScale());

		ComponentAsteroid* asteroid = (ComponentAsteroid*)entity->AddComponent(Component::Type::ASTEROID);
		asteroid->direction = glm::vec2(1.0f, 0.0f);
		asteroid->speed = 100.0f;
	}
}

void ModuleScene::DeleteAsteroids(int num)
{
	for (int i = 0; i < num; ++i)
		DeleteEntity(entities.front());
}
