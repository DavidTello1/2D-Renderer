#pragma once
#include "System.h"
#include "FixedGrid.h"

#include "glm/include/glm/glm.hpp"

#define GRID_CELLSIZE 500

enum class CollisionDirection {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT,
    ERROR
};

enum class CollisionType {
    ERROR = 0,
    CIRCLE_CIRCLE,
    CIRCLE_RECT,
    RECT_CIRCLE,
    RECT_RECT
};

struct CollidingPair
{
    EntityIdx entity1;
    EntityIdx entity2;

    glm::vec2 distance;
    CollisionType type;
};

struct EventWorldSizeUpdate;

struct C_Collider;
struct C_Transform;
struct C_RigidBody;

class S_Physics : public System
{
public:
    S_Physics();
    ~S_Physics();

    void Init() override;
    void Update(float dt) override;

    FixedGrid* GetGrid() { return grid; }
    const FixedGrid* GetGrid() const { return grid; }
    void SetGridCellSize(const float& cellSize) { grid->SetCellSize(cellSize);  ResizeGrid(grid->GetPosition(), grid->GetSize(), cellSize); }

private:
    bool CheckCollision(const C_Collider& collider1, const C_Collider& collider2, glm::vec2& distance, CollisionType& type);

    void CollisionCircleRect(const CollidingPair pair);
    void CollisionCircleCircle(const CollidingPair pair);
    void CollisionRectRect(const CollidingPair pair);

    bool PairExists(const CollidingPair& new_pair, const std::vector<CollidingPair>& list) const;
    CollisionDirection GetCollisionDirection(glm::vec2 distance) const;

    // Fixed Resolution Grid
    void ResizeGrid(glm::vec2 pos, glm::vec2 size, float cellSize);
    void RecalculateGrid();

    // --- EVENTS ---
    void OnWorldSizeUpdate(EventWorldSizeUpdate* e);

private:
    std::vector<C_Transform> transforms;
    std::vector<C_Collider> colliders;
    std::vector<C_RigidBody> rigidbodies;

    FixedGrid* grid = nullptr;
};
