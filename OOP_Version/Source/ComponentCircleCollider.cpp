#include "ComponentCircleCollider.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleScene.h"
#include "ModulePhysics.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentSprite.h"
#include "ComponentRectCollider.h"

ComponentCircleCollider::ComponentCircleCollider(Entity* entity) :
	Component(Component::Type::CIRCLE_COLLIDER, entity), center(0.0f), radius(0.0f), offset(0.0f)
{
	App->physics->AddCollider(this);
	collision.Reset();
}

ComponentCircleCollider::~ComponentCircleCollider()
{
	App->physics->RemoveCollider(this);
}

void ComponentCircleCollider::OnUpdate(float dt)
{
	collision.Reset();

	Entity* entity = GetEntity();
	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	ComponentSprite* sprite = (ComponentSprite*)entity->GetComponent(Component::Type::SPRITE);
	if (sprite == nullptr)
		return;

	glm::vec2 size = sprite->GetSize();
	glm::vec2 pos = transform->GetPosition();
	center = glm::vec2(pos + radius + offset);

	// Check for Collisions
	DoCollisions();
}

glm::vec2 ComponentCircleCollider::GetPosition()
{
	glm::vec2 pos = glm::vec2(0.0f);

	Entity* entity = GetEntity();
	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform != nullptr)
		pos = transform->GetPosition() + offset;

	return pos;
}

bool ComponentCircleCollider::CheckCollision(ComponentCircleCollider* collider) // Circle-Circle Collision
{
	glm::vec2 dist = this->center - collider->center;
	float distance = glm::sqrt(dist.x * dist.x + dist.y * dist.y);

	if (distance < this->radius + collider->radius) 
	{
		this->collision.has_collided = true;
		this->collision.distance = dist;
		this->collision.type = collider->GetType();
		this->collision.other_radius = collider->GetRadius();
		return true;
	}
	return false;
}

bool ComponentCircleCollider::CheckCollision(ComponentRectCollider* collider) // Circle-Rect Collision
{
	// Find the closest point to the circle within the rectangle
	glm::vec2 closest = glm::clamp(this->center, collider->GetPosition(), collider->GetPosition() + collider->GetSize());

	// Calculate the distance between the circle's center and this closest point
	glm::vec2 dist = this->center - closest;

	// If the distance is less than the circle's radius, an intersection occurs
	float distanceSquared = (dist.x * dist.x) + (dist.y * dist.y);
	if (distanceSquared < (this->radius * this->radius))
	{
		this->collision.has_collided = true;
		this->collision.distance = dist;
		this->collision.type = collider->GetType();
		return true;
	}
	return false;
}

void ComponentCircleCollider::DoCollisions()
{
	for (Entity* entity : App->scene->GetEntities())
	{
		if (entity == this->GetEntity())
			continue;

		ComponentCircleCollider* cCollider = (ComponentCircleCollider*)entity->GetComponent(Component::Type::CIRCLE_COLLIDER);
		if (cCollider != nullptr)
		{
			CheckCollision(cCollider);
		}
		else
		{
			ComponentRectCollider* rCollider = (ComponentRectCollider*)entity->GetComponent(Component::Type::RECT_COLLIDER);
			if (rCollider != nullptr)
			{
				CheckCollision(rCollider);
			}
		}
	}
}
