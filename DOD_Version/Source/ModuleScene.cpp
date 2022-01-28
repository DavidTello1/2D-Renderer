#include "ModuleScene.h"

#include "Application.h"
#include "ModuleWindow.h"

#include "Components.h"
#include "Systems.h"

ModuleScene::ModuleScene(bool start_enabled) : Module("ModuleScene", start_enabled)
{
	pcg32_srandom_r(&rng, 42u, 54u); // Seed the RNG with a fixed constant
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	//--- Init Scene Manager ---
	coordinator.Init();

	//--- Register Components ---
	coordinator.RegisterComponent<C_Transform>();
	coordinator.RegisterComponent<C_Sprite>();
	coordinator.RegisterComponent<C_Renderer>();
	coordinator.RegisterComponent<C_Camera>();
	coordinator.RegisterComponent<C_CameraController>();
	coordinator.RegisterComponent<C_RigidBody>();
	coordinator.RegisterComponent<C_CircleCollider>();
	coordinator.RegisterComponent<C_RectCollider>();

	//--- Register Systems and Init ---
	// Renderer
	render_system = coordinator.RegisterSystem<S_Renderer>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<C_Transform>());
		signature.set(coordinator.GetComponentType<C_Renderer>());
		signature.set(coordinator.GetComponentType<C_Sprite>());
		coordinator.SetSystemSignature<S_Renderer>(signature);
	}

	// Camera Controller
	camera_system = coordinator.RegisterSystem<S_CameraController>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<C_Transform>());
		signature.set(coordinator.GetComponentType<C_Camera>());
		signature.set(coordinator.GetComponentType<C_CameraController>());
		coordinator.SetSystemSignature<S_CameraController>(signature);
	}
	camera_system->Init();

	// Physics
	physics_system = coordinator.RegisterSystem<S_Physics>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<C_Transform>());
		signature.set(coordinator.GetComponentType<C_RigidBody>());
		signature.set(coordinator.GetComponentType<C_CircleCollider>()); //***
		signature.set(coordinator.GetComponentType<C_RectCollider>()); //***
		coordinator.SetSystemSignature<S_Physics>(signature);
	}
	physics_system->Init();

	return true;
}

bool ModuleScene::Start()
{
	return true;
}

bool ModuleScene::Update(float dt)
{
	// Update Systems
	camera_system->Update(dt);
	physics_system->Update(dt);

	//// Check if asteroid is out of bounds
	//for (size_t i = 0; i < entities.size(); ++i)
	//{
	//	if (entities[i]->GetComponent(Component::Type::ASTEROID) != nullptr)
	//	{
	//		ComponentTransform* transform = (ComponentTransform*)entities[i]->GetComponent(Component::Type::TRANSFORM);
	//		glm::vec2 pos = transform->GetPosition();
	//		glm::vec2 size = transform->GetSize() * transform->GetScale();
	
	//		if (pos.x + size.x < 0 || pos.x > world_width ||
	//			pos.y + size.y < 0 || pos.y > world_height)
	//			DeleteEntity(entities[i]);
	//	}
	//}

	return true;
}

bool ModuleScene::CleanUp()
{
	//while(!entities.empty())
	//	DeleteEntity(entities.front());
	//entities.clear();

	return true;
}

void ModuleScene::Draw()
{
	render_system->Render();
}

void ModuleScene::OnResize(int width, int height)
{
	camera_system->OnResize(width, height);
}

void ModuleScene::OnZoom(int new_zoom)
{
	camera_system->OnZoom(new_zoom);
}
