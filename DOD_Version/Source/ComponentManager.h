#pragma once
#include "ComponentArray.h"

#include <unordered_map>
#include <memory>

typedef std::uint8_t ComponentType;

class ComponentManager
{
public:
    template<typename T> void AddComponent(EntityIdx entity, T component) { GetComponentArray<T>()->InsertData(entity, component); }
    template<typename T> void RemoveComponent(EntityIdx entity) { GetComponentArray<T>()->RemoveData(entity); }
    template<typename T> T& GetComponent(EntityIdx entity) { return GetComponentArray<T>()->GetData(entity); }

    template<typename T>
    void RegisterComponent() {
        const char* type = typeid(T).name();
        assert(component_types.find(type) == component_types.end() && "Registering component type more than once.");

        component_types.insert({ type, count_types });
        component_arrays.insert({ type, std::make_shared<ComponentArray<T>>() });
        ++count_types;
    }

    template<typename T>
    ComponentType GetComponentType() {
        const char* type = typeid(T).name();
        assert(component_types.find(type) != component_types.end() && "Component not registered before use.");

        return component_types[type];
    }

    void EntityDestroyed(EntityIdx entity) { //*** EVENT SYSTEM
        for (auto const& pair : component_arrays)
        {
            auto const& component = pair.second;
            component->EntityDestroyed(entity);
        }
    }

private:
    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray() {
        const char* type = typeid(T).name();
        assert(component_types.find(type) != component_types.end() && "Component not registered before use.");

        return std::static_pointer_cast<ComponentArray<T>>(component_arrays[type]);
    }

private:
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> component_arrays;

    std::unordered_map<const char*, ComponentType> component_types;
    ComponentType count_types;
};