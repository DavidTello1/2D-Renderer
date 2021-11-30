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
	main_camera = (ComponentCamera*)cam->AddComponent(Component::Type::CAMERA);

	return true;
}

bool ModuleScene::Start()
{
	// Create Background
	Entity* bg = CreateEntity();
	bg->AddComponent(Component::Type::RENDERER);
	ComponentTransform* transform = (ComponentTransform*)bg->AddComponent(Component::Type::TRANSFORM);
	transform->SetPosition(glm::vec2(100.0, 200.0f));
	//transform->SetScale(glm::vec2(WORLD_SCALE));
	background = (ComponentSprite*)bg->AddComponent(Component::Type::SPRITE);
	background->SetTexture(App->resources->LoadTexture("Assets/background.png")->index);
	//background->SetSize(glm::vec2(world_width, world_height));

	//// Create World Boundaries
	//Entity* top = CreateEntity();
	//top->AddComponent(Component::Type::RENDERER);
	//transform = (ComponentTransform*)top->AddComponent(Component::Type::TRANSFORM);
	//b_top = (ComponentRectCollider*)top->AddComponent(Component::Type::RECT_COLLIDER);
	//transform->SetPosition(glm::vec2(-BOUNDARIES_SIZE, -BOUNDARIES_SIZE));
	//b_top->SetSize(glm::vec2(world_width * WORLD_SCALE + 2 * BOUNDARIES_SIZE, BOUNDARIES_SIZE));

	//Entity* bottom = CreateEntity();
	//bottom->AddComponent(Component::Type::RENDERER);
	//transform = (ComponentTransform*)bottom->AddComponent(Component::Type::TRANSFORM);
	//b_bottom = (ComponentRectCollider*)bottom->AddComponent(Component::Type::RECT_COLLIDER);
	//transform->SetPosition(glm::vec2(-BOUNDARIES_SIZE, world_height * WORLD_SCALE));
	//b_bottom->SetSize(glm::vec2(world_width * WORLD_SCALE + 2 * BOUNDARIES_SIZE, BOUNDARIES_SIZE));

	//Entity* left = CreateEntity();
	//left->AddComponent(Component::Type::RENDERER);
	//transform = (ComponentTransform*)left->AddComponent(Component::Type::TRANSFORM);
	//b_left = (ComponentRectCollider*)left->AddComponent(Component::Type::RECT_COLLIDER);
	//transform->SetPosition(glm::vec2(-BOUNDARIES_SIZE, 0.0f));
	//b_left->SetSize(glm::vec2(BOUNDARIES_SIZE, world_height * WORLD_SCALE));

	//Entity* right = CreateEntity();
	//right->AddComponent(Component::Type::RENDERER);
	//transform = (ComponentTransform*)right->AddComponent(Component::Type::TRANSFORM);
	//b_right = (ComponentRectCollider*)right->AddComponent(Component::Type::RECT_COLLIDER);
	//transform->SetPosition(glm::vec2(world_width * WORLD_SCALE, 0.0f));
	//b_right->SetSize(glm::vec2(BOUNDARIES_SIZE, world_height * WORLD_SCALE));

	// ---- Asteroid Testing ---
	Entity* entity = CreateEntity();
	entity->AddComponent(Component::Type::RENDERER);

	ComponentTransform* transf = (ComponentTransform*)entity->AddComponent(Component::Type::TRANSFORM);
	transf->SetPosition(glm::vec2(0.0f, 0.0f));
	transf->SetScale(glm::vec2(200.0f));

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
	for (Entity* entity : entities)
		entity->Draw(App->resources->default_shader);
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

void ModuleScene::DrawDebug(uint shader)
{
	for (Entity* entity : entities)
	{
		ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
		if (transform == nullptr)
			return;

		ComponentCircleCollider* collider = (ComponentCircleCollider*)entity->GetComponent(Component::Type::CIRCLE_COLLIDER);
		if (collider != nullptr)
		{
			glm::vec4 color;
			if (collider->GetCollision() == nullptr)
				color = BLUE;
			else
				color = RED;
			color.a = 0.5f;

			glUniformMatrix4fv(glGetUniformLocation(shader, "uViewProj"), 1, GL_FALSE, (GLfloat*)&App->scene->main_camera->GetViewProjMatrix());
			glUniformMatrix4fv(glGetUniformLocation(shader, "uTransform"), 1, GL_FALSE, (GLfloat*)&transform->GetTransform());
			glUniform1iv(glGetUniformLocation(shader, "uTexture"), 1, (GLint*)&App->resources->default_tex);

			//App->renderer->DrawCircle(collider->GetCenter(), collider->GetRadius(), color);
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

				glUniformMatrix4fv(glGetUniformLocation(shader, "uViewProj"), 1, GL_FALSE, (GLfloat*)&App->scene->main_camera->GetViewProjMatrix());
				glUniformMatrix4fv(glGetUniformLocation(shader, "uTransform"), 1, GL_FALSE, (GLfloat*)&transform->GetTransform());
				glUniform1iv(glGetUniformLocation(shader, "uTexture"), 1, (GLint*)&App->resources->default_tex);

				//App->renderer->DrawQuad(collider->GetPosition(), collider->GetSize(), color);
			}
		}
	}
}

void ModuleScene::UpdateWorldSize()
{
	// Update Background Size
	background->SetSize(glm::vec2(world_width, world_height));

	// Update Boundaries Size
	b_top->SetSize(glm::vec2(world_width * WORLD_SCALE + 2 * BOUNDARIES_SIZE, BOUNDARIES_SIZE));
	b_bottom->SetSize(glm::vec2(world_width * WORLD_SCALE + 2 * BOUNDARIES_SIZE, BOUNDARIES_SIZE));
	b_left->SetSize(glm::vec2(BOUNDARIES_SIZE, world_height * WORLD_SCALE));
	b_right->SetSize(glm::vec2(BOUNDARIES_SIZE, world_height * WORLD_SCALE));
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
		DeleteEntity(entities[BASE_ENTITIES - 1]); // start from asteroids position in vector to not delete base entities
}
