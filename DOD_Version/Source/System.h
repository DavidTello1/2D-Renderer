#pragma once
#include "Globals.h"

#include <bitset>
#include <vector>

typedef std::bitset<MAX_COMPONENTS> ComponentMask;

class System
{
public:
    virtual void Init() {};
    virtual void Start() {};
    virtual void Update(float dt) {};
    virtual void Render() {};

    const ComponentMask& GetSignature() { return mask; }

    void AddEntity(const EntityIdx& entity) { 
        entities.push_back(entity); 
    }

    void RemoveEntity(const EntityIdx& entity) {
        for (size_t i = 0; i < entities.size(); ++i)
        {
            if (entity == entities[i])
            {
                entities.erase(entities.begin() + i);
                return;
            }
        }
    }

public:
    ComponentMask mask;

    std::vector<EntityIdx> entities;
};
