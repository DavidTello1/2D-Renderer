#include "ComponentAsteroid.h"

#include "Application.h"
#include "ModuleSceneBase.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentCircleCollider.h"

#include "PCG/pcg_basic.h"

void ComponentAsteroid::Move(float dt)
{
	Entity* entity = GetEntity();

	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	ComponentCircleCollider* collider = (ComponentCircleCollider*)entity->GetComponent(Component::Type::CIRCLE_COLLIDER);
	if (collider == nullptr)
		return;

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

	// Size & Mass
	ComponentTransform* transform = (ComponentTransform*)GetEntity()->GetComponent(Component::Type::TRANSFORM);
	ComponentCircleCollider* collider = (ComponentCircleCollider*)GetEntity()->GetComponent(Component::Type::CIRCLE_COLLIDER);
	if (transform == nullptr || collider == nullptr) return;

	int size = pcg32_boundedrand_r(&App->scene_base->GetRNG(), MAX_SIZE + 1);
	if (size < MIN_SIZE)
		size = MIN_SIZE;
	mass = size / 10.0f;
	transform->SetSize(glm::vec2(size));
	collider->SetRadius(transform->GetSize().x * transform->GetScale().x / 2);
}