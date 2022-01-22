#include "ModuleScene.h"

#include "Components.h"
#include "Systems.h"

ModuleScene::ModuleScene(bool start_enabled) : Module("ModuleScene", start_enabled)
{
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
		signature.set(coordinator.GetComponentType<C_Renderer>());
		signature.set(coordinator.GetComponentType<C_Sprite>());
		signature.set(coordinator.GetComponentType<C_Transform>());
		coordinator.SetSystemSignature<S_Renderer>(signature);
	}

	// Camera Controller
	camera_system = coordinator.RegisterSystem<S_CameraController>();
	{
		Signature signature;
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

	return true;
}

bool ModuleScene::CleanUp()
{
	return true;
}

void ModuleScene::Draw()
{
	render_system->Render();
}
