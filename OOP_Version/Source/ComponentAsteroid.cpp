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

	DoCollisions(collider, transform);

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

const int& ComponentAsteroid::GetDirection(glm::vec2 distance) const
{
	float max = 0.0f;
	int direction = -1;

	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};

	for (uint i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(distance), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			direction = i;
		}
	}
	return direction;
}

void ComponentAsteroid::DoCollisions(ComponentCircleCollider* collider, ComponentTransform* transform)
{
	if (collider->IsColliding() == false || collider->IsStatic() == true)
		return;

	Collision collision = collider->GetCollision();
	int direction = GetDirection(collision.distance);
	float distance = glm::sqrt(collision.distance.x * collision.distance.x + collision.distance.y * collision.distance.y);

	if (collider->GetCollision().type == Component::Type::RECT_COLLIDER)
	{
		float overlap = collider->GetRadius() - distance; // Get Overlap
		if (std::isnan(overlap)) overlap = 0;

		if (overlap > 0)
		{
			glm::vec2 final_pos = transform->GetPosition();

			if (direction == 0) // UP
			{
				velocity.y = -velocity.y;
				final_pos.y += overlap;
			}
			else if (direction == 1) // RIGHT
			{
				velocity.x = -velocity.x;
				final_pos.x += overlap;
			}
			else if (direction == 3) // LEFT
			{
				velocity.x = -velocity.x;
				final_pos.x -= overlap;
			}
			else if (direction == 2)// DOWN
			{
				velocity.y = -velocity.y;
				final_pos.y -= overlap;
			}

			transform->SetPosition(final_pos);
		}
	}
	else if (collider->GetCollision().type == Component::Type::CIRCLE_COLLIDER)
	{
		float overlap = 0.5f * (distance - collider->GetRadius() - collision.other_radius);

		// Displace Current Ball away from collision
		glm::vec2 center = collider->GetCenter();
		center -= overlap * collision.distance / distance;
		collider->SetCenter(center);

		glm::vec2 final_pos = glm::vec2(center - collider->GetRadius() - collider->GetOffset());
		transform->SetPosition(final_pos);

		if (direction == 3 || direction == 1) // horizontal collision
		{
			velocity.x = -velocity.x;
		}
		else // vertical collision
		{
			velocity.y = -velocity.y; // reverse vertical velocity
		}
	}
}
