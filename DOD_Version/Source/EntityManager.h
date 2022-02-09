#pragma once

#include <array>
#include <queue>
#include <bitset>
#include <cassert>

const int MAX_ENTITIES = 10000;
const int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> ComponentMask;
typedef unsigned int EntityIdx;

class EntityManager
{
public:
    EntityManager() {
        for (EntityIdx entity = 0; entity < MAX_ENTITIES; ++entity)
            available_indexes.push(entity);
    }

    EntityIdx CreateEntity() {
        assert(count_entities < MAX_ENTITIES && "Too many entities in existence.");

        EntityIdx index = available_indexes.front();
        available_indexes.pop();
        ++count_entities;

        return index;
    }

    void DestroyEntity(EntityIdx entity) {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        component_masks[entity].reset();
        available_indexes.push(entity);
        --count_entities;
    }

    void SetMask(EntityIdx entity, ComponentMask mask) {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        component_masks[entity] = mask;
    }

    ComponentMask GetMask(EntityIdx entity) {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        return component_masks[entity];
    }

private:
    std::array<ComponentMask, MAX_ENTITIES> component_masks;

    std::queue<EntityIdx> available_indexes;
    uint32_t count_entities = 0;
};