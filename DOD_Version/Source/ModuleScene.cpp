#include "ModuleScene.h"

#include "Components.h"
#include "S_Renderer.h"
#include "S_Debug.h"
#include "S_CameraController.h"
#include "S_Physics.h"

#include "Optick/include/optick.h"
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
	//--- Init Entity Manager ---
	for (EntityIdx entity = 0; entity < MAX_ENTITIES; ++entity)
		available_indexes.push(entity);

	//--- Register Components ---
	RegisterComponentMgr<C_Transform>();
	RegisterComponentMgr<C_Sprite>();
	RegisterComponentMgr<C_Renderer>();
	RegisterComponentMgr<C_Camera>();
	RegisterComponentMgr<C_CameraController>();
	RegisterComponentMgr<C_RigidBody>();
	RegisterComponentMgr<C_Collider>();

	//--- Register Systems and Init ---
	systems.push_back(render_system = new S_Renderer());
	systems.push_back(debug_system = new S_Debug());
	systems.push_back(camera_system = new S_CameraController());
	systems.push_back(physics_system = new S_Physics());

	for (size_t i = 0, size = systems.size(); i < size; ++i)
		systems[i]->Init();

	return true;
}

bool ModuleScene::Start()
{
	for (size_t i = 0, size = systems.size(); i < size; ++i)
		systems[i]->Start();

	return true;
}

bool ModuleScene::Update(float dt)
{
	OPTICK_CATEGORY("Scene Update", Optick::Category::Scene);

	// Update Systems
	OPTICK_PUSH("Camera System");
	camera_system->Update(dt);
	OPTICK_POP();

	OPTICK_PUSH("Physics System");
	physics_system->Update(dt);
	OPTICK_POP();

	return true;
}

bool ModuleScene::CleanUp()
{
	// Delete Component Managers
	for (int i = (int)ComponentType::TYPE_COUNT - 1; i >= 0; --i)
		RELEASE(component_mgrs[i]);

	// Delete Systems
	for (int i = systems.size() - 1; i >= 0; --i)
		RELEASE(systems[i]);

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

//--------------------------------------
// --- ENTITY COMPONENT SYSTEM ---
