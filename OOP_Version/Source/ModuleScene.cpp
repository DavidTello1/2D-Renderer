#include "ModuleScene.h"

#include "Application.h"
#include "ModuleSceneBase.h"
#include "ModuleResources.h"
#include "ModuleRenderer.h"

#include "Entity.h"
#include "ComponentCamera.h"
#include "ComponentCameraController.h"
#include "ComponentTransform.h"
#include "ComponentSprite.h"
#include "ComponentRectCollider.h"
#include "ComponentCircleCollider.h"
#include "ComponentAsteroid.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

#include "mmgr/mmgr.h"

ModuleScene::ModuleScene(bool start_enabled) : Module("ModuleScene", start_enabled), world_width(WORLD_WIDTH), world_height(WORLD_HEIGHT)
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	// Create Main Camera
	Entity* cam = CreateEntity();
	cam->AddComponent(Component::Type::CAMERA);
	ComponentCameraController* controller = (ComponentCameraController*)cam->AddComponent(Component::Type::CAMERA_CONTROLLER);
	App->scene_base->SetMainCamera(controller);

	return true;
}

bool ModuleScene::Start()
{
	// Create Background & Boundaries
	Entity* bg = CreateEntity();
	bg->AddComponent(Component::Type::RENDERER);
	background = (ComponentTransform*)bg->AddComponent(Component::Type::TRANSFORM);
	ComponentSprite* sprite = (ComponentSprite*)bg->AddComponent(Component::Type::SPRITE);
	//sprite->SetTexture(App->resources->LoadTexture("Assets/background.png")->index);
	sprite->SetTexture(App->resources->LoadTexture("Assets/black.png")->index);

	b_top = CreateEntity();
	b_top->AddComponent(Component::Type::TRANSFORM);
	b_top->AddComponent(Component::Type::RECT_COLLIDER);

	b_bottom = CreateEntity();
	b_bottom->AddComponent(Component::Type::TRANSFORM);
	b_bottom->AddComponent(Component::Type::RECT_COLLIDER);

	b_left = CreateEntity();
	b_left->AddComponent(Component::Type::TRANSFORM);
	b_left->AddComponent(Component::Type::RECT_COLLIDER);

	b_right = CreateEntity();
	b_right->AddComponent(Component::Type::TRANSFORM);
	b_right->AddComponent(Component::Type::RECT_COLLIDER);

	UpdateWorldSize();

	return true;
}

bool ModuleScene::Update(float dt)
{
	for (Entity* entity : entities)
	{
		for(Component* component : entity->GetComponents())
			component->OnUpdate(dt);
	}

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
	for (Entity* entity : entities)
		entity->Draw();
}

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
void ModuleScene::UpdateWorldSize()
{
	// Update Background
	background->SetSize(glm::vec2(world_width, world_height));

	// Update Boundaries
	ComponentTransform* transform = (ComponentTransform*)b_top->GetComponent(Component::Type::TRANSFORM);
	transform->SetPosition(glm::vec2(-BOUNDARIES_SIZE, -BOUNDARIES_SIZE));
	transform->SetSize(glm::vec2(world_width + 2 * BOUNDARIES_SIZE, BOUNDARIES_SIZE));

	transform = (ComponentTransform*)b_bottom->GetComponent(Component::Type::TRANSFORM);
	transform->SetPosition(glm::vec2(-BOUNDARIES_SIZE, world_height));
	transform->SetSize(glm::vec2(world_width + 2 * BOUNDARIES_SIZE, BOUNDARIES_SIZE));

	transform = (ComponentTransform*)b_left->GetComponent(Component::Type::TRANSFORM);
	transform->SetPosition(glm::vec2(-BOUNDARIES_SIZE, 0.0f));
	transform->SetSize(glm::vec2(BOUNDARIES_SIZE, world_height));

	transform = (ComponentTransform*)b_right->GetComponent(Component::Type::TRANSFORM);
	transform->SetPosition(glm::vec2(world_width, 0.0f));
	transform->SetSize(glm::vec2(BOUNDARIES_SIZE, world_height));

	// Update Grid
	App->renderer->UpdateGrid();
}

void ModuleScene::AddAsteroids(int num)
{
	for (int i = 0; i < num; ++i)
	{
		Entity* entity = CreateEntity();
		entity->AddComponent(Component::Type::RENDERER);

		ComponentTransform* transform = (ComponentTransform*)entity->AddComponent(Component::Type::TRANSFORM);
		transform->SetSize(glm::vec2(100.0f));

		ComponentSprite* sprite = (ComponentSprite*)entity->AddComponent(Component::Type::SPRITE);
		int tex = pcg32_boundedrand_r(&App->scene_base->GetRNG(), 3);
		if (tex == 0)
			sprite->SetTexture(App->resources->LoadTexture("Assets/asteroid_grey.png")->index);
		else if(tex == 1)
			sprite->SetTexture(App->resources->LoadTexture("Assets/asteroid_red.png")->index);
		else if (tex == 2)
			sprite->SetTexture(App->resources->LoadTexture("Assets/asteroid_brown.png")->index);

		ComponentCircleCollider* collider = (ComponentCircleCollider*)entity->AddComponent(Component::Type::CIRCLE_COLLIDER);
		collider->SetRadius(transform->GetSize().x * transform->GetScale().x / 2);

		ComponentAsteroid* asteroid = (ComponentAsteroid*)entity->AddComponent(Component::Type::ASTEROID);
		asteroid->SetRandomValues();

		// Position Limits
		glm::vec2 pos;
		pos.x = pcg32_boundedrand_r(&App->scene_base->GetRNG(), world_width - transform->GetSize().x * transform->GetScale().x - BOUNDARIES_SIZE);
		pos.y = pcg32_boundedrand_r(&App->scene_base->GetRNG(), world_height - transform->GetSize().y * transform->GetScale().y - BOUNDARIES_SIZE);

		if (pos.x == 0) pos.x = BOUNDARIES_SIZE;
		if (pos.y == 0) pos.y = BOUNDARIES_SIZE;

		transform->SetPosition(pos);
	}
}

void ModuleScene::DeleteAsteroids(int num) //*** HAS TO BE FIXED
{
	for (int i = 0; i < num; ++i)
		DeleteEntity(entities[BASE_ENTITIES - 1]); // start from asteroids position in vector to not delete base entities
}
