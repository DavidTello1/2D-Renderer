#pragma once
#include "Module.h"

#include "ComponentManager.h"

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true) : Module("ModuleScene", start_enabled) {
		entity_mgr = std::make_unique<EntityManager>();
		component_mgr = std::make_unique<ComponentManager>();
		//system_mgr = std::make_unique<SystemManager>();
	};
	virtual ~ModuleScene() {};

	//--- ENTITIES ---
	Entity CreateEntity() { return entity_mgr->CreateEntity(); }

	void DeleteEntity(Entity entity) {
		entity_mgr->DeleteEntity(entity);

		//Event EntityDeleted;
		//component_mgr->RemoveEntity(entity)
		//system_mgr->RemoveEntity(entity)
	}

	//--- COMPONENTS ---
	template<typename T>
	void RegisterComponent() { component_mgr->RegisterComponent<T>(); }

	template<typename T>
	T& GetComponent(Entity entity) { return component_mgr->GetComponent<T>(entity); }

	template<typename T>
	std::uint8_t GetComponentType() { return component_mgr->GetComponentType<T>(); }

	template<typename T>
	void AddComponent(Entity entity, T component) {
		component_mgr->AddComponent<T>(entity, component);

		//*** Update Entity Mask 
		//*** Update Systems
	}

	template<typename T>
	void RemoveComponent(Entity entity) {
		component_mgr->RemoveComponent<T>(entity);

		//*** Update Entity Mask 
		//*** Update Systems
	}

	//--- SYSTEMS ---
	//...


private:
	std::unique_ptr<EntityManager> entity_mgr;
	std::unique_ptr<ComponentManager> component_mgr;
};
