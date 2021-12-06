#include "ComponentRectCollider.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleScene.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentCircleCollider.h"

void ComponentRectCollider::OnUpdate(float dt)
{
	is_colliding = false;

	Entity* entity = GetEntity();
	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	position = transform->GetPosition() + offset;

	// Check for Collisions
	DoCollisions();
}

bool ComponentRectCollider::CheckCollision(ComponentRectCollider* collider) // Rect-Rect Collision
{
	if (this->position.x < collider->position.x + collider->size.x && this->position.x + this->size.x > collider->position.x &&
		this->position.y < collider->position.y + collider->size.y && this->position.y + this->size.y > collider->position.y)
	{
		is_colliding = true;
		return true;

	}
	return false;
}

bool ComponentRectCollider::CheckCollision(ComponentCircleCollider* collider) // Circle-Rect Collision
{
	// Find the closest point to the circle within the rectangle
	float closestX = glm::clamp(collider->GetCenter().x, this->position.x, this->position.x + this->size.x);
	float closestY = glm::clamp(collider->GetCenter().y, this->position.y, this->position.y + this->size.y);

	// Calculate the distance between the circle's center and this closest point
	float distanceX = collider->GetCenter().x - closestX;
	float distanceY = collider->GetCenter().y - closestY;

	// If the distance is less than the circle's radius, an intersection occurs
	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
	if (distanceSquared < (collider->GetRadius() * collider->GetRadius()))
	{
		is_colliding = true;
		return true;
	}
	return false;
}

void ComponentRectCollider::DoCollisions()
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