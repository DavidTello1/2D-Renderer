#include "ModuleScene.h"

#include "Components.h"
#include "S_Renderer.h"
#include "S_Debug.h"
#include "S_CameraController.h"
#include "S_Physics.h"

#include "mmgr/mmgr.h"

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
	mComponentManager = std::make_unique<ComponentManager>();
	mEntityManager = std::make_unique<EntityManager>();
	mSystemManager = std::make_unique<SystemManager>();

	//--- Register Components ---
	RegisterComponent<C_Transform>();
	RegisterComponent<C_Sprite>();
	RegisterComponent<C_Renderer>();
	RegisterComponent<C_Camera>();
	RegisterComponent<C_CameraController>();
	RegisterComponent<C_RigidBody>();
	RegisterComponent<C_Collider>();

	//--- Register Systems and Init ---
	// Renderer
	render_system = RegisterSystem<S_Renderer>();
	{
		ComponentMask signature;
		signature.set(GetComponentType<C_Transform>());
		signature.set(GetComponentType<C_Renderer>());
		signature.set(GetComponentType<C_Sprite>());
		SetSystemSignature<S_Renderer>(signature);
	}

	// Debug
	debug_system = RegisterSystem<S_Debug>();
	{
		ComponentMask signature;
		signature.set(GetComponentType<C_Collider>());
		SetSystemSignature<S_Debug>(signature);
	}
	debug_system->Init();

	// Camera Controller
	camera_system = RegisterSystem<S_CameraController>();
	{
		ComponentMask signature;
		signature.set(GetComponentType<C_Transform>());
		signature.set(GetComponentType<C_Camera>());
		signature.set(GetComponentType<C_CameraController>());
		SetSystemSignature<S_CameraController>(signature);
	}
	camera_system->Init();

	// Physics
	physics_system = RegisterSystem<S_Physics>();
	{
		ComponentMask signature;
		signature.set(GetComponentType<C_Transform>());
		signature.set(GetComponentType<C_RigidBody>());
		signature.set(GetComponentType<C_Collider>());
		SetSystemSignature<S_Physics>(signature);
	}
	physics_system->Init();

	return true;
}

bool ModuleScene::Start()
{
	debug_system->Start();

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
	//while(!entities.empty())
	//	DeleteEntity(entities.front());
	//entities.clear();

	return true;
}

//--------------------------------------
void ModuleScene::Draw()
{
	render_system->Render();
}

void ModuleScene::DrawDebug(bool grid, bool colliders)
{
	if (grid)
		debug_system->RenderGrid();

	if (colliders)
		debug_system->RenderColliders();
}