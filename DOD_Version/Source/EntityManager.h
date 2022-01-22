#pragma once

#include <array>
#include <queue>
#include <bitset>
#include <cassert>

const int MAX_ENTITIES = 10000;
const int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;
typedef unsigned int Entity;

class EntityManager
{
public:
    EntityManager()
    {
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        {
            mAvailableEntities.push(entity);
        }
    }

    Entity CreateEntity()
    {
        assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

        Entity id = mAvailableEntities.front();
        mAvailableEntities.pop();
        ++mLivingEntityCount;

        return id;
    }

    void DestroyEntity(Entity entity)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        mSignatures[entity].reset();
        mAvailableEntities.push(entity);
        --mLivingEntityCount;
    }

    void SetSignature(Entity entity, Signature signature)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        mSignatures[entity] = signature;
    }

    Signature GetSignature(Entity entity)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        return mSignatures[entity];
    }

private:
    std::queue<Entity> mAvailableEntities{};
    std::array<Signature, MAX_ENTITIES> mSignatures{};
    uint32_t mLivingEntityCount{};
};