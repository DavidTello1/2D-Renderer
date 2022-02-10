#include "ModuleScene.h"

#include "Components.h"
#include "Systems.h"

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
	RegisterComponent<C_CircleCollider>();
	RegisterComponent<C_RectCollider>();

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
		signature.set(GetComponentType<C_CircleCollider>()); //***
		signature.set(GetComponentType<C_RectCollider>()); //***
		SetSystemSignature<S_Physics>(signature);
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

//--------------------------------------
void ModuleScene::Draw()
{
	render_system->Render();
}
