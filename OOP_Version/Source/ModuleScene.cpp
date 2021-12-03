#include "ModuleScene.h"

#include "Application.h"
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
	main_camera = (ComponentCameraController*)cam->AddComponent(Component::Type::CAMERA_CONTROLLER);

	return true;
}

bool ModuleScene::Start()
{
	// Create Background & Boundaries
	Entity* bg = CreateEntity();
	bg->AddComponent(Component::Type::RENDERER);
	ComponentTransform* transform = (ComponentTransform*)bg->AddComponent(Component::Type::TRANSFORM);
	transform->SetPosition(glm::vec2(0.0f, 0.0f));
	background = (ComponentSprite*)bg->AddComponent(Component::Type::SPRITE);
	background->SetTexture(App->resources->LoadTexture("Assets/background.png")->index);

	b_top = CreateEntity();
	b_top->AddComponent(Component::Type::RENDERER);
	b_top->AddComponent(Component::Type::TRANSFORM);
	b_top->AddComponent(Component::Type::RECT_COLLIDER);

	b_bottom = CreateEntity();
	b_bottom->AddComponent(Component::Type::RENDERER);
	b_bottom->AddComponent(Component::Type::TRANSFORM);
	b_bottom->AddComponent(Component::Type::RECT_COLLIDER);

	b_left = CreateEntity();
	b_left->AddComponent(Component::Type::RENDERER);
	b_left->AddComponent(Component::Type::TRANSFORM);
	b_left->AddComponent(Component::Type::RECT_COLLIDER);

	b_right = CreateEntity();
	b_right->AddComponent(Component::Type::RENDERER);
	b_right->AddComponent(Component::Type::TRANSFORM);
	b_right->AddComponent(Component::Type::RECT_COLLIDER);

	UpdateWorldSize();

	// ---- Asteroid Testing ---
	Entity* entity = CreateEntity();
	entity->AddComponent(Component::Type::RENDERER);

	ComponentTransform* transf = (ComponentTransform*)entity->AddComponent(Component::Type::TRANSFORM);
	transf->SetPosition(glm::vec2(200.0f));

	ComponentSprite* sprite = (ComponentSprite*)entity->AddComponent(Component::Type::SPRITE);
	sprite->SetTexture(App->resources->LoadTexture("Assets/asteroids.png")->index);
	sprite->SetSize(glm::vec2(100.0f));

	ComponentCircleCollider* collider = (ComponentCircleCollider*)entity->AddComponent(Component::Type::CIRCLE_COLLIDER);
	collider->SetRadius(sprite->GetSize().x * transform->GetScale().x / 2);

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
void ModuleScene::DrawAxis()
{
}

void ModuleScene::DrawGrid(float grid_size)
{

}

const glm::mat4& ModuleScene::GetViewProjMatrix() const
{
	return App->scene->main_camera->GetCamera()->GetViewProjMatrix();
}

void ModuleScene::DrawDebug()
{
	for (Entity* entity : entities)
	{
		ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
		if (transform == nullptr)
			continue;

		ComponentCircleCollider* collider = (ComponentCircleCollider*)entity->GetComponent(Component::Type::CIRCLE_COLLIDER);
		if (collider != nullptr)
		{
			glm::vec4 color;
			if (collider->GetCollision() == nullptr)
				color = BLUE;
			else
				color = RED;
			color.a = 0.5f;

			App->renderer->DrawCircle(App->resources->default_shader, collider->GetCenter(), collider->GetRadius(), color);
		}
		else
		{
			ComponentRectCollider* collider = (ComponentRectCollider*)entity->GetComponent(Component::Type::RECT_COLLIDER);
			if (collider != nullptr)
			{
				glm::vec4 color;
				if (collider->GetCollision() == nullptr)
					color = BLUE;
				else
					color = RED;
				color.a = 0.5f;

				App->renderer->DrawQuad(App->resources->default_shader, collider->GetPosition(), collider->GetSize(), color);
			}
		}
	}
}

void ModuleScene::UpdateWorldSize()
{
	// Update Background
	background->SetSize(glm::vec2(world_width, world_height ));

	// Update Boundaries
	ComponentTransform* transform = (ComponentTransform*)b_top->GetComponent(Component::Type::TRANSFORM);
	ComponentRectCollider* collider = (ComponentRectCollider*)b_top->GetComponent(Component::Type::RECT_COLLIDER);
	transform->SetPosition(glm::vec2(-BOUNDARIES_SIZE, -BOUNDARIES_SIZE));
	collider->SetSize(glm::vec2(world_width + 2 * BOUNDARIES_SIZE, BOUNDARIES_SIZE));

	transform = (ComponentTransform*)b_bottom->GetComponent(Component::Type::TRANSFORM);
	collider = (ComponentRectCollider*)b_bottom->GetComponent(Component::Type::RECT_COLLIDER);
	transform->SetPosition(glm::vec2(-BOUNDARIES_SIZE, world_height));
	collider->SetSize(glm::vec2(world_width + 2 * BOUNDARIES_SIZE, BOUNDARIES_SIZE));

	transform = (ComponentTransform*)b_left->GetComponent(Component::Type::TRANSFORM);
	collider = (ComponentRectCollider*)b_left->GetComponent(Component::Type::RECT_COLLIDER);
	transform->SetPosition(glm::vec2(-BOUNDARIES_SIZE, 0.0f));
	collider->SetSize(glm::vec2(BOUNDARIES_SIZE, world_height));

	transform = (ComponentTransform*)b_right->GetComponent(Component::Type::TRANSFORM);
	collider = (ComponentRectCollider*)b_right->GetComponent(Component::Type::RECT_COLLIDER);
	transform->SetPosition(glm::vec2(world_width, 0.0f));
	collider->SetSize(glm::vec2(BOUNDARIES_SIZE, world_height));
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

		ComponentCircleCollider* collider = (ComponentCircleCollider*)entity->AddComponent(Component::Type::CIRCLE_COLLIDER);
		collider->SetRadius(sprite->GetSize().x * transform->GetScale().x / 2);

		ComponentAsteroid* asteroid = (ComponentAsteroid*)entity->AddComponent(Component::Type::ASTEROID);
		asteroid->direction = glm::vec2(1.0f, 0.0f);
		asteroid->speed = 100.0f;
	}
}

void ModuleScene::DeleteAsteroids(int num)
{
	for (int i = 0; i < num; ++i)
		DeleteEntity(entities[BASE_ENTITIES - 1]); // start from asteroids position in vector to not delete base entities
}
