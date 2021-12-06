#include "ComponentAsteroid.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentCircleCollider.h"

#include <math.h>

void ComponentAsteroid::OnUpdate(float dt)
{
	Entity* entity = GetEntity();

	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	ComponentCircleCollider* collider = (ComponentCircleCollider*)entity->GetComponent(Component::Type::CIRCLE_COLLIDER);
	if (collider == nullptr)
		return;

	if (collider->IsColliding() && !collider->isStatic)
	{
		if (collider->isBoundariesCollision)
			OnCollision(collider, transform);
		else
			OnCollision2(collider, transform);
	}

	transform->SetPosition(transform->GetPosition() * direction + velocity * dt);
}

void ComponentAsteroid::SetRandomValues()
{
	//direction = ...
	//speed = ...
}

void ComponentAsteroid::OnCollision(ComponentCircleCollider* collider, ComponentTransform* transform)
{
	// Get Direction
	float max = 0.0f;
	uint direction = -1;
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	for (uint i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(collider->dist), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			direction = i;
		}
	}

				  // LEFT			// RIGHT
	if (direction == 3 || direction == 1) // horizontal collision
	{
		velocity.x = -velocity.x; // reverse horizontal velocity

		float distance = glm::sqrt(collider->dist.x * collider->dist.x + collider->dist.y * collider->dist.y);
		float overlap = collider->GetRadius() - distance;
		if (std::isnan(overlap)) overlap = 0;
		if (overlap > 0)
		{
			glm::vec2 pos = transform->GetPosition();
			glm::vec2 tmp_pos = pos;
			if (direction == 3) // LEFT
			{
				pos.x -= overlap;
				transform->SetPosition(pos);
			}
			else // RIGHT
			{
				pos.x = pos.x + overlap;
				transform->SetPosition(pos);
			}
		}
	}
	else // vertical collision
	{
		velocity.y = -velocity.y; // reverse vertical velocity

		float distance = glm::sqrt(collider->dist.x * collider->dist.x + collider->dist.y * collider->dist.y);
		float overlap = collider->GetRadius() - distance;
		if (std::isnan(overlap)) overlap = 0;
		if (overlap > 0)
		{
			glm::vec2 pos = transform->GetPosition();
			if (direction == 0) // UP
			{
				pos.y += overlap;
				transform->SetPosition(pos);
			}
			else // DOWN
			{
				pos.y -= overlap;
				transform->SetPosition(pos);
			}
		}
	}
}

void ComponentAsteroid::OnCollision2(ComponentCircleCollider* collider, ComponentTransform* transform)
{
	// --- Static Collisions (fixes overlaping) ---

	// Distance between ball centers
	float distance = glm::sqrt(collider->dist.x * collider->dist.x + collider->dist.y * collider->dist.y);

	// Calculate displacement required
	float overlap = 0.5f * (distance - collider->GetRadius() - collider->radius_collision);

	// Displace Current Ball away from collision
	glm::vec2 center = collider->GetCenter();
	center.x -= overlap * collider->dist.x / distance;
	center.y -= overlap * collider->dist.y / distance;
	collider->SetCenter(center);

	glm::vec2 pos1 = glm::vec2(collider->GetCenter() - collider->GetRadius() - collider->GetOffset());
	transform->SetPosition(pos1);


	// Get Direction
	float max = 0.0f;
	uint direction = -1;
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	for (uint i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(collider->dist), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			direction = i;
		}
	}

				  // LEFT			// RIGHT
	if (direction == 3 || direction == 1) // horizontal collision
	{
		velocity.x = -velocity.x; // reverse horizontal velocity
	}
	else // vertical collision
	{
		velocity.y = -velocity.y; // reverse vertical velocity
	}
}
