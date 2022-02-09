#pragma once

#include "EntityManager.h"

#include <array>
#include <unordered_map>
#include <cassert>

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(EntityIdx entity) = 0;
};


template<typename T>
class ComponentArray : public IComponentArray
{
public:
    void InsertData(EntityIdx entity, T component) {
        assert(entity_to_index.find(entity) == entity_to_index.end() && "Component added to same entity more than once.");

        // Put new entry at end
        size_t newIndex = count_components;
        entity_to_index[entity] = newIndex;
        index_to_entity[newIndex] = entity;
        components[newIndex] = component;
        ++count_components;
    }

    void RemoveData(EntityIdx entity) {
        assert(entity_to_index.find(entity) != entity_to_index.end() && "Removing non-existent component.");

        // Copy element at end into deleted element's place to maintain density
        size_t indexOfRemovedEntity = entity_to_index[entity];
        size_t indexOfLastElement = count_components - 1;
        components[indexOfRemovedEntity] = components[indexOfLastElement];

        // Update map to point to moved spot
        EntityIdx entityOfLastElement = index_to_entity[indexOfLastElement];
        entity_to_index[entityOfLastElement] = indexOfRemovedEntity;
        index_to_entity[indexOfRemovedEntity] = entityOfLastElement;

        entity_to_index.erase(entity);
        index_to_entity.erase(indexOfLastElement);

        --count_components;
    }

    T& GetData(EntityIdx entity) {
        assert(entity_to_index.find(entity) != entity_to_index.end() && "Retrieving non-existent component.");

        return components[entity_to_index[entity]];
    }

    void EntityDestroyed(EntityIdx entity) override { //*** EVENT SYSTEM
        if (entity_to_index.find(entity) != entity_to_index.end())
        {
            RemoveData(entity);
        }
    }

private:
    std::array<T, MAX_ENTITIES> components;

    std::unordered_map<EntityIdx, size_t> entity_to_index;
    std::unordered_map<size_t, EntityIdx> index_to_entity;
    size_t count_components;
};