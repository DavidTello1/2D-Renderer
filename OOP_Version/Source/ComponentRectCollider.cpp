#include "ComponentRectCollider.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleScene.h"
#include "ModulePhysics.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentCircleCollider.h"

ComponentRectCollider::ComponentRectCollider(Entity* entity) :
	Component(Component::Type::RECT_COLLIDER, entity), position(0.0f), size(0.0f), offset(0.0f)
{
	App->physics->AddCollider(this);
	collision.Reset();
}

ComponentRectCollider::~ComponentRectCollider()
{
	App->physics->RemoveCollider(this);
}

void ComponentRectCollider::OnUpdate(float dt)
{
	collision.Reset();

	Entity* entity = GetEntity();
	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	position = transform->GetPosition() + offset;

	//// Check for Collisions
	//DoCollisions();
}

bool ComponentRectCollider::CheckCollision(ComponentRectCollider* collider) // Rect-Rect Collision
{
	if (this->position.x < collider->position.x + collider->size.x && this->position.x + this->size.x > collider->position.x &&
		this->position.y < collider->position.y + collider->size.y && this->position.y + this->size.y > collider->position.y)
	{
		this->collision.has_collided = true;
		this->collision.type = collider->GetType();
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
		this->collision.has_collided = true;
		this->collision.type = collider->GetType();
		return true;
	}
	return false;
}

void ComponentRectCollider::DoCollisions()
{
	for (Entity* entity : App->scene->GetEntities())
	{
		if (entity == this->GetEntity())
			continue;

		ComponentCircleCollider* cCollider = (ComponentCircleCollider*)entity->GetComponent(Component::Type::CIRCLE_COLLIDER);
		if (cCollider != nullptr)
		{
			if (CheckCollision(cCollider))
				return;
		}
		else
		{
			ComponentRectCollider* rCollider = (ComponentRectCollider*)entity->GetComponent(Component::Type::RECT_COLLIDER);
			if (rCollider != nullptr)
			{
				if (CheckCollision(rCollider))
					return;
			}
		}
	}
}
