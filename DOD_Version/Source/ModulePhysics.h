#pragma once
#include "Module.h"

#include "glm/include/glm/glm.hpp"
#include <vector>

struct C_Collider;

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

class ModulePhysics : public Module
{
public:
	ModulePhysics(bool start_enabled = true) : Module("ModulePhysics", start_enabled) {};
	virtual ~ModulePhysics() {};

	bool Init() override;
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	bool CheckCollision(const C_Collider& collider1, const C_Collider& collider2, glm::vec2& distance, CollisionType& type);
	CollisionDirection GetCollisionDirection(glm::vec2 distance) const;
};

