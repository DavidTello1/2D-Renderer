#include "ComponentAsteroid.h"

#include "Application.h"
#include "ModuleSceneBase.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentCircleCollider.h"

#include "PCG/pcg_basic.h"

void ComponentAsteroid::OnUpdate(float dt)
{
	Entity* entity = GetEntity();

	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	ComponentCircleCollider* collider = (ComponentCircleCollider*)entity->GetComponent(Component::Type::CIRCLE_COLLIDER);
	if (collider == nullptr)
		return;

	if (collider->IsColliding() && !collider->IsStatic())
	{
		if (collider->isBoundariesCollision)
			OnCollision(collider, transform);
		else
			OnCollision2(collider, transform);
	}

	transform->SetPosition(transform->GetPosition() + velocity * dt);
	transform->SetRotation(transform->GetRotation() + rotation_speed * dt);
}

void ComponentAsteroid::SetRandomValues()
{
	// Rotation
	rotation_speed = pcg32_boundedrand_r(&App->scene_base->GetRNG(), MAX_ROTATION_SPEED + 1);
	if (rotation_speed < MIN_ROTATION_SPEED)
		rotation_speed = MIN_ROTATION_SPEED;

	if (pcg32_boundedrand_r(&App->scene_base->GetRNG(), 2) == 0) // orientation
		rotation_speed *= -1;

	// Velocity X
	velocity.x = pcg32_boundedrand_r(&App->scene_base->GetRNG(), MAX_VELOCITY + 1);
	if (velocity.x < MIN_VELOCITY)
		velocity.x = MIN_VELOCITY;

	if (pcg32_boundedrand_r(&App->scene_base->GetRNG(), 2) == 0) // orientation
		velocity.x *= -1;

	// Velocity Y
	velocity.y = pcg32_boundedrand_r(&App->scene_base->GetRNG(), MAX_VELOCITY + 1);
	if (velocity.y < MIN_VELOCITY)
		velocity.y = MIN_VELOCITY;

	if (pcg32_boundedrand_r(&App->scene_base->GetRNG(), 2) == 0) // orientation
		velocity.y *= -1;
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
