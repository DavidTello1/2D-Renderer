#include "ModuleGame.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleResources.h"
#include "ModuleScene.h"
#include "ModuleEvent.h"

#include "Components.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

#include "mmgr/mmgr.h"

ModuleGame::ModuleGame(bool start_enabled) : Module("ModuleGame", start_enabled), world_width(DEFAULT_WORLD_WIDTH), world_height(DEFAULT_WORLD_HEIGHT)
{
}

ModuleGame::~ModuleGame()
{
}

bool ModuleGame::Init()
{
	// Create Main Camera
	C_Transform transform = { glm::vec2(0.0f), glm::vec2(1.0f), 0.0f, glm::vec2(0.0f) };
	C_Camera camera = { glm::ortho(0.0f, (float)App->window->GetWidth(), (float)App->window->GetHeight(), 0.0f) };
	C_CameraController controller = { 1.0f, 200.0f, 0.25f };

	main_camera = App->scene->CreateEntity();
	App->scene->AddComponent(main_camera, transform);
	App->scene->AddComponent(main_camera, camera);
	App->scene->AddComponent(main_camera, controller);

	return true;
}

bool ModuleGame::Start()
{
	// Create Background
	C_Transform transform = { glm::vec2(0.0f), glm::vec2(1.0f), 0.0f, glm::vec2(0.0f) };
	C_Sprite sprite = {
		App->resources->default_shader,
		App->resources->LoadTexture("Assets/background.png")->index
	};
	background = App->scene->CreateEntity();
	App->scene->AddComponent(background, C_Renderer{ true });
	App->scene->AddComponent(background, transform);
	App->scene->AddComponent(background, sprite);

	// Create Boundaries
	transform = { glm::vec2(0.0f), glm::vec2(1.0f), 0.0f, glm::vec2(0.0f) };
	C_RectCollider collider = { false, true, glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f) };

	b_top = App->scene->CreateEntity();
	App->scene->AddComponent(b_top, transform);
	App->scene->AddComponent(b_top, collider);

	b_bottom = App->scene->CreateEntity();
	App->scene->AddComponent(b_bottom, transform);
	App->scene->AddComponent(b_bottom, collider);

	b_left = App->scene->CreateEntity();
	App->scene->AddComponent(b_left, transform);
	App->scene->AddComponent(b_left, collider);

	b_right = App->scene->CreateEntity();
	App->scene->AddComponent(b_right, transform);
	App->scene->AddComponent(b_right, collider);

	// Update World Size
	UpdateWorldSize();

	return true;
}

//--------------------------------------
const glm::mat4& ModuleGame::GetViewProjMatrix() const
{
	C_Transform transform = App->scene->GetComponent<C_Transform>(main_camera);
	glm::mat4 projection = App->scene->GetComponent<C_Camera>(main_camera).projection;

	glm::mat4 view = glm::translate(glm::mat4(1.0f), { transform.position, 0.0f }) *
		glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation), glm::vec3(0, 0, 1));
	view = glm::inverse(view);

	glm::mat4 viewproj = projection * view;
	return viewproj;
}

//--------------------------------------
void ModuleGame::UpdateWorldSize()
{
	//--- Update Background
	C_Transform& transform = App->scene->GetComponent<C_Transform>(background);
	transform.size = glm::vec2(world_width, world_height);

	//--- Update Boundaries
	// Top
	C_Transform& transform1 = App->scene->GetComponent<C_Transform>(b_top);
	C_RectCollider& collider1 = App->scene->GetComponent<C_RectCollider>(b_top);
	transform1.position = glm::vec2(-BOUNDARIES_SIZE, -BOUNDARIES_SIZE);
	transform1.size = glm::vec2(world_width + 2 * BOUNDARIES_SIZE, BOUNDARIES_SIZE);
	collider1.position = transform1.position;
	collider1.size = transform1.size;

	// Bottom
	C_Transform& transform2 = App->scene->GetComponent<C_Transform>(b_bottom);
	C_RectCollider& collider2 = App->scene->GetComponent<C_RectCollider>(b_bottom);
	transform2.position = glm::vec2(-BOUNDARIES_SIZE, world_height);
	transform2.size = glm::vec2(world_width + 2 * BOUNDARIES_SIZE, BOUNDARIES_SIZE);
	collider2.position = transform2.position;
	collider2.size = transform2.size;

	// Left
	C_Transform& transform3 = App->scene->GetComponent<C_Transform>(b_left);
	C_RectCollider& collider3 = App->scene->GetComponent<C_RectCollider>(b_left);
	transform3.position = glm::vec2(-BOUNDARIES_SIZE, 0.0f);
	transform3.size = glm::vec2(BOUNDARIES_SIZE, world_height);
	collider3.position = transform3.position;
	collider3.size = transform3.size;

	// Right
	C_Transform& transform4 = App->scene->GetComponent<C_Transform>(b_right);
	C_RectCollider& collider4 = App->scene->GetComponent<C_RectCollider>(b_right);
	transform4.position = glm::vec2(world_width, 0.0f);
	transform4.size = glm::vec2(BOUNDARIES_SIZE, world_height);
	collider4.position = transform4.position;
	collider4.size = transform4.size;

	// Send Event to Update Grid
	App->event_mgr->Publish(new EventWorldSizeUpdate(world_width, world_height));
}

void ModuleGame::AddAsteroids(int num)
{
	for (int i = 0; i < num; ++i)
	{
		Entity entity = App->scene->CreateEntity();
		App->scene->AddComponent(entity, C_Renderer{ true });

		// --- Sprite
		C_Sprite sprite;
		sprite.shader = App->resources->default_shader;

		int tex = pcg32_boundedrand_r(&App->scene->GetRNG(), 3);
		if (tex == 0)
			sprite.texture = App->resources->LoadTexture("Assets/asteroid_1.png")->index;
		else if(tex == 1)
			sprite.texture = App->resources->LoadTexture("Assets/asteroid_2.png")->index;
		else if (tex == 2)
			sprite.texture = App->resources->LoadTexture("Assets/asteroid_3.png")->index;

		App->scene->AddComponent(entity, sprite);

		// --- Transform
		C_Transform transform;
		transform.rotation = 0.0f;
		transform.scale = glm::vec2(1.0f);

		transform.size = glm::vec2(pcg32_boundedrand_r(&App->scene->GetRNG(), MAX_ASTEROID_SIZE + 1));
		if (transform.size.x < MIN_ASTEROID_SIZE) // Limits
			transform.size = glm::vec2(MIN_ASTEROID_SIZE);

		transform.position.x = pcg32_boundedrand_r(&App->scene->GetRNG(), world_width - transform.size.x * transform.scale.x - BOUNDARIES_SIZE);
		if (transform.position.x < 0) // Limits
			transform.position.x = BOUNDARIES_SIZE;

		transform.position.y = pcg32_boundedrand_r(&App->scene->GetRNG(), world_height - transform.size.y * transform.scale.y - BOUNDARIES_SIZE);
		if (transform.position.y < 0) // Limits
			transform.position.y = BOUNDARIES_SIZE;

		App->scene->AddComponent(entity, transform);

		//// --- Collider
		//C_CircleCollider collider;
		//collider.is_colliding = false;
		//collider.is_static = false;
		//collider.center = transform.position;
		//collider.offset = glm::vec2(0.0f);
		//collider.radius = transform.size.x * transform.scale.x / 2;
		//App->scene->AddComponent(entity, collider);

		// --- Rigidbody
		C_RigidBody rigidbody;
		rigidbody.mass = transform.size.x / 10.0f;

		rigidbody.velocity.x = pcg32_boundedrand_r(&App->scene->GetRNG(), MAX_ASTEROID_VELOCITY + 1);
		if (rigidbody.velocity.x < MIN_ASTEROID_VELOCITY) // Limits
			rigidbody.velocity.x = MIN_ASTEROID_VELOCITY;
		if (pcg32_boundedrand_r(&App->scene->GetRNG(), 2) == 0) // Orientation
			rigidbody.velocity.x *= -1;

		rigidbody.velocity.y = pcg32_boundedrand_r(&App->scene->GetRNG(), MAX_ASTEROID_VELOCITY + 1);
		if (rigidbody.velocity.y < MIN_ASTEROID_VELOCITY) // Limits
			rigidbody.velocity.y = MIN_ASTEROID_VELOCITY;
		if (pcg32_boundedrand_r(&App->scene->GetRNG(), 2) == 0) // Orientation
			rigidbody.velocity.y *= -1;

		rigidbody.rotation_speed = pcg32_boundedrand_r(&App->scene->GetRNG(), MAX_ASTEROID_ROTATION_SPEED + 1);
		if (rigidbody.rotation_speed < MIN_ASTEROID_ROTATION_SPEED) // Limits
			rigidbody.rotation_speed = MIN_ASTEROID_ROTATION_SPEED;
		if (pcg32_boundedrand_r(&App->scene->GetRNG(), 2) == 0) // Orientation
			rigidbody.rotation_speed *= -1;

		// orientation
		App->scene->AddComponent(entity, rigidbody);
	}
}

void ModuleGame::DeleteAsteroids(int num)
{
	//for (int i = 0; i < num; ++i)
	//	DeleteEntity(entities[BASE_ENTITIES]); // start from asteroids position in vector to not delete base entities
}
