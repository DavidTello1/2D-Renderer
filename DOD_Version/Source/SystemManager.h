#pragma once
#include "EntityManager.h"
#include "SystemBase.h"

#include <unordered_map>
#include <memory>
#include <cassert>

class SystemManager
{
public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem() {
        const char* typeName = typeid(T).name();
        assert(systems.find(typeName) == systems.end() && "Registering system more than once.");

        auto system = std::make_shared<T>();
        systems.insert({ typeName, system });
        return system;
    }

    template<typename T>
    void SetMask(ComponentMask signature)
    {
        const char* typeName = typeid(T).name();
        assert(systems.find(typeName) != systems.end() && "System used before registered.");

        component_masks.insert({ typeName, signature });
    }

    void EntityDestroyed(EntityIdx entity)
    {
        for (auto const& pair : systems)
        {
            auto const& system = pair.second;
            system->entities.erase(entity);
        }
    }

    void EntitySignatureChanged(EntityIdx entity, ComponentMask entitySignature)
    {
        for (auto const& pair : systems)
        {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = component_masks[type];

            if ((entitySignature & systemSignature) == systemSignature)
                system->entities.insert(entity);
            else
                system->entities.erase(entity);
        }
    }

private:
    std::unordered_map<const char*, std::shared_ptr<SystemBase>> systems;
    std::unordered_map<const char*, ComponentMask> component_masks;
};