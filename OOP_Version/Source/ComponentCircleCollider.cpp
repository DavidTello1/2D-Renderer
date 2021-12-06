#include "ComponentCircleCollider.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleScene.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentSprite.h"
#include "ComponentRectCollider.h"

void ComponentCircleCollider::OnUpdate(float dt)
{
	is_colliding = false;

	Entity* entity = GetEntity();
	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	ComponentSprite* sprite = (ComponentSprite*)entity->GetComponent(Component::Type::SPRITE);
	if (sprite == nullptr)
		return;

	glm::vec2 pos = transform->GetPosition();
	glm::vec2 size = sprite->GetSize();

	center = glm::vec2(pos.x + radius + offset.x, pos.y + radius + offset.y);

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
	float dx = this->center.x - collider->center.x;
	float dy = this->center.y - collider->center.y;
	float distance = glm::sqrt(dx * dx + dy * dy);

	if (distance < this->radius + collider->radius) 
	{
		is_colliding = true;
		return true;
	}
	return false;
}

bool ComponentCircleCollider::CheckCollision(ComponentRectCollider* collider) // Circle-Rect Collision
{
	// Find the closest point to the circle within the rectangle
	float closestX = glm::clamp(this->center.x, collider->GetPosition().x, collider->GetPosition().x + collider->GetSize().x);
	float closestY = glm::clamp(this->center.y, collider->GetPosition().y, collider->GetPosition().y + collider->GetSize().y);

	// Calculate the distance between the circle's center and this closest point
	float distanceX = this->center.x - closestX;
	float distanceY = this->center.y - closestY;

	// If the distance is less than the circle's radius, an intersection occurs
	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
	if (distanceSquared < (this->radius * this->radius))
	{
		is_colliding = true;
		return true;
	}
	return false;
}

void ComponentCircleCollider::DoCollisions()
{
	for (Entity* entity : App->scene->entities)
	{
		if (entity == this->GetEntity())
			continue;

		ComponentCircleCollider* cCollider = (ComponentCircleCollider*)entity->GetComponent(Component::Type::CIRCLE_COLLIDER);
		if (cCollider != nullptr)
		{
			// Has Circle Collider
			if (CheckCollision(cCollider) == true) //has collided
			{
				//...
				return;
			}
		}
		else
		{
			ComponentRectCollider* rCollider = (ComponentRectCollider*)entity->GetComponent(Component::Type::RECT_COLLIDER);
			if (rCollider != nullptr)
			{
				// Has Rect Collider
				if (CheckCollision(rCollider) == true) //has collided
				{
					// Bounce
					return;
				}
			}
		}
	}
}
