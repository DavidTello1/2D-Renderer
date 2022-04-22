#pragma once
#include "Globals.h"

#include <array>

class BaseComponentManager
{
public:
    virtual ~BaseComponentManager() = default;
};

template<typename Component>
class ComponentManager : public BaseComponentManager
{
public:
    void AddComponent(EntityIdx entity, Component& component)
    {
        if (entity >= MAX_ENTITIES)
            return;

        components[n] = component;
        dense[n] = entity;
        sparse[entity] = n;
        ++n;
    };

    void RemoveComponent(EntityIdx entity)
    {
        if (HasComponent(entity))
        {
            --n;
            Component data = components[n];
            uint32_t item = dense[n];
            uint32_t dense_index = sparse[entity];

            components[dense_index] = data;
            dense[dense_index] = item;
            sparse[data] = dense_index;
        }
    }

    bool HasComponent(EntityIdx entity)
    {
        if (entity >= MAX_ENTITIES)
            return false;

        return sparse[entity] < n;
    }

    Component& GetComponent(EntityIdx entity)
    {
        uint32_t index = sparse[entity];
        return components[index];
    }

    const Component& GetComponent(EntityIdx entity) const
    {
        uint32_t index = sparse[entity];
        return const components[index];
    }

    size_t GetSize() const 
    {
        return n;
    }

private:
    std::array<Component, MAX_ENTITIES> components; // actual data, order is the same as dense

    std::array<uint32_t, MAX_ENTITIES> dense;
    std::array<uint32_t, MAX_ENTITIES> sparse;
    uint n;

    //// Disallow this to be copied by mistake
    //ComponentManager(const ComponentManager&) = delete;
};