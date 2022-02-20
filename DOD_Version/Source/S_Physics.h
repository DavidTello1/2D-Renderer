#pragma once
#include "System.h"
#include "ModulePhysics.h"

struct CollidingPair
{
    EntityIdx entity1;
    EntityIdx entity2;

    glm::vec2 distance;
    CollisionType type;
};

class S_Physics : public System
{
public:
    void Init();
    void Update(float dt);

private:
    void CollisionCircleRect(const CollidingPair pair);
    void CollisionCircleCircle(const CollidingPair pair);
    void CollisionRectRect(const CollidingPair pair);

    bool PairExists(const CollidingPair& new_pair, const std::vector<CollidingPair>& list) const;
};
