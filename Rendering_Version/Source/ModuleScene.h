#pragma once
#include "Module.h"

#include "Application.h"
#include "ModuleEvent.h"

#include "ComponentManager.h"
#include "Components.h"

#include "System.h"

#include "PCG/pcg_basic.h"
#include <array>
#include <queue>
#include <bitset>
#include <memory>

typedef std::bitset<MAX_COMPONENTS> ComponentMask;

class S_Renderer;
class S_Debug;
class S_CameraController;
class S_Physics;

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	virtual ~ModuleScene();

	bool Init() override;
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	pcg32_random_t& GetRNG() { return rng; }
	
    void Draw();
    void DrawGrid();
    void DrawColliders();


	// --- ENTITY COMPONENT SYSTEM ---
    // --- ENTITIES ---
    EntityIdx CreateEntity() 
    {
        EntityIdx index = available_indexes.front();
        available_indexes.pop();
        ++count_entities;
        return index;
    }

    void DeleteEntity(EntityIdx entity) 
    {
        component_masks[entity].reset();
        available_indexes.push(entity);
        --count_entities;

        App->event_mgr->Publish(new EventEntityDestroyed(entity));
        EntityMaskUpdated(entity, component_masks[entity]);
    }

    int GetEntityCount() { return count_entities; }


    // --- COMPONENTS ---
    template<typename T>
    void RegisterComponentMgr() 
    {
        int type = GetComponentType<T>(); // Add type to list
        component_mgrs[type] = new ComponentManager<T>(); // create new manager and add to list
    }

    template<typename T>
    void AddComponent(const EntityIdx& entity, T& component)
    {
        ComponentManager<T>* mgr = GetComponentManager<T>();
        mgr->AddComponent(entity, component);

        component_masks[entity].set(GetComponentType<T>(), true);
        EntityMaskUpdated(entity, component_masks[entity]);
    }

    template<typename T>
    void RemoveComponent(EntityIdx entity)
    {
        ComponentManager<T>* mgr = GetComponentManager<T>();
        mgr->RemoveComponent(entity);

        component_masks[entity].set(GetComponentType<T>(), false);
        EntityMaskUpdated(entity, component_masks[entity]);
    }

    template<typename T>
    T& GetComponent(EntityIdx entity)
    {
        ComponentManager<T>* mgr = GetComponentManager<T>();
        return mgr->GetComponent(entity);
    }

    template<typename T>
    bool HasComponent(EntityIdx entity)
    {
        ComponentManager<T>* mgr = GetComponentManager<T>();
        return mgr->HasComponent(entity);
    }

    template<typename T>
    void SetComponent(EntityIdx entity, T& component)
    {
        ComponentManager<T>* mgr = GetComponentManager<T>();
        mgr->SetComponent(entity, component);
    }


    // --- SYSTEMS ---
    void EntityMaskUpdated(const EntityIdx& entity, const ComponentMask& mask)
    { 
        for (System* system : systems)
        {
            if ((mask & system->GetSignature()) == system->GetSignature())
                system->AddEntity(entity);
            else
                system->RemoveEntity(entity);
        }
    }


    //------------------------------
    template<typename T>
    int GetComponentType() { assert(0 && "No ID for this component type"); }

    template<> int GetComponentType<C_Transform>() { return (int)ComponentType::C_TRANSFORM; }
    template<> int GetComponentType<C_Sprite>() { return (int)ComponentType::C_SPRITE; }
    template<> int GetComponentType<C_Renderer>() { return (int)ComponentType::C_RENDERER; }
    template<> int GetComponentType<C_Camera>() { return (int)ComponentType::C_CAMERA; }
    template<> int GetComponentType<C_CameraController>() { return (int)ComponentType::C_CAMERACONTROLLER; }
    template<> int GetComponentType<C_RigidBody>() { return (int)ComponentType::C_RIGIDBODY; }
    template<> int GetComponentType<C_Collider>() { return (int)ComponentType::C_COLLIDER; }

private:
    template<typename T>
    ComponentManager<T>* GetComponentManager() 
    {
        int type = GetComponentType<T>();
        return (ComponentManager<T>*)component_mgrs[type];
    }

private:
	pcg32_random_t rng;

	// --- Entities ---
    std::array<ComponentMask, MAX_ENTITIES> component_masks;

    std::queue<EntityIdx> available_indexes;
    uint32_t count_entities = 0;

    // --- Components ---
    std::array<BaseComponentManager*, MAX_COMPONENTS> component_mgrs;

	// --- Systems ---
    std::vector<System*> systems;

    S_Renderer* render_system = nullptr;
    S_Debug* debug_system = nullptr;
	S_CameraController* camera_system = nullptr;
	S_Physics* physics_system = nullptr;
};
