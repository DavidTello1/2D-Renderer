#pragma once
#include "Module.h"

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
    void DrawDebug(bool grid, bool colliders);


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

        //for (BaseComponentManager* mgr : component_mgrs)
        //    mgr->EntityDestroyed(entity); //*** EVENT

        for (System* system : systems)
            system->RemoveEntity(entity);
    }

    int GetEntityCount() { return count_entities; }


    // --- COMPONENTS ---
    template<typename T>
    void RegisterComponent() 
    {
        const char* type = typeid(T).name(); // Get type as string
        type = strstr(type, "_") + 1;

        component_types[count_types] = type; // Add type to list
        component_mgrs[count_types] = new ComponentManager<T>(); // create new manager and add to list
        count_types++;
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
    int GetComponentType()
    {        
        const char* type = typeid(T).name(); // Get type as string
        type = strstr(type, "_") + 1;

        for (size_t i = 0; i < count_types; ++i)
        {
            if (strcmp(component_types[i], type) == 0)
                return (int)i;
        }
        return -1;
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
    std::array<const char*, MAX_COMPONENTS> component_types;
    uint32_t count_types = 0;

	// --- Systems ---
    std::vector<System*> systems;

	S_Renderer* render_system;
    S_Debug* debug_system;
	S_CameraController* camera_system;
	S_Physics* physics_system;
};
