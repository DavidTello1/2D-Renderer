#include "ModulePhysics.h"

#include "Application.h"
#include "ModuleGame.h"
#include "ModuleRenderer.h"
#include "ModuleResources.h"

#include "EntityManager.h"
#include "Components.h"

#include "Optick/include/optick.h"
#include "mmgr/mmgr.h"

bool ModulePhysics::Init()
{
	return true;
}

bool ModulePhysics::Start()
{
	return true;
}

bool ModulePhysics::Update(float dt)
{
	return true;
}

bool ModulePhysics::CleanUp()
{
	return true;
}

//----------------------
CollisionDirection& ModulePhysics::GetCollisionDirection(glm::vec2 distance) const
{
	float max = 0.0f;
	CollisionDirection direction = CollisionDirection::ERROR;

	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(-1.0f, 0.0f)	// left
	};

	for (uint i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(distance), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			direction = CollisionDirection(i);
		}
	}
	return direction;
}

bool ModulePhysics::CheckCollision(const C_Collider& collider1, const C_Collider& collider2, glm::vec2& distance, CollisionType& type)
{
	switch (collider1.type)
	{
	// --- CIRCLE ---
	case C_Collider::ColliderType::CIRCLE:
	{
		switch (collider2.type)
		{
		case C_Collider::ColliderType::CIRCLE: // Circle - Circle
		{
			type = CollisionType::CIRCLE_CIRCLE;
			distance = collider1.center - collider2.center;
			return ((distance.x * distance.x) + (distance.y * distance.y) < (collider1.radius + collider2.radius) * (collider1.radius + collider2.radius));
		}
		case C_Collider::ColliderType::RECT: // Circle - Rect
		{
			type = CollisionType::CIRCLE_RECT;
			distance = collider1.center - glm::clamp(collider1.center, collider2.position, collider2.position + collider2.size);
			return ((distance.x * distance.x) + (distance.y * distance.y) < (collider1.radius * collider1.radius));
		}
		case C_Collider::ColliderType::UNKNOWN: {
			LOG("---Error: Collider Type Unknown");
			type = CollisionType::ERROR;
			return false;
		}
		}
	}

	// --- RECT ---
	case C_Collider::ColliderType::RECT: 
	{
		switch (collider2.type)
		{
		case C_Collider::ColliderType::CIRCLE: // Rect - Circle
		{
			type = CollisionType::RECT_CIRCLE;
			distance = collider2.center - glm::clamp(collider2.center, collider1.position, collider1.position + collider1.size);
			return ((distance.x * distance.x) + (distance.y * distance.y) < (collider2.radius * collider2.radius));
		}
		case C_Collider::ColliderType::RECT: // Rect - Rect
		{
			type = CollisionType::RECT_RECT;
			distance = glm::vec2(0.0f); //*** GET ACTUAL VALUE
			return (collider1.position.x < collider2.position.x + collider2.size.x && 
					collider1.position.x + collider1.size.x > collider2.position.x && 
					collider1.position.y < collider2.position.y + collider2.size.y && 
					collider1.position.y + collider1.size.y > collider2.position.y);
		}
		case C_Collider::ColliderType::UNKNOWN: { 
			LOG("---Error: Collider Type Unknown");
			type = CollisionType::ERROR;
			return false; }
		}
	}

	case C_Collider::ColliderType::UNKNOWN: 
	{
		LOG("---Error: Collider Type Unknown");
		type = CollisionType::ERROR;
		return false;
	}
	}
}
