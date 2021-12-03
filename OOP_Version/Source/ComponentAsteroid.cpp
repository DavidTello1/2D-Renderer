#include "ComponentAsteroid.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentCircleCollider.h"

void ComponentAsteroid::OnUpdate(float dt)
{
	Entity* entity = GetEntity();

	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	ComponentCircleCollider* collider = (ComponentCircleCollider*)entity->GetComponent(Component::Type::CIRCLE_COLLIDER);
	if (collider == nullptr)
		return;

	if (collider->IsColliding())
	{
		OnCollision();
	}

	transform->SetPosition(transform->GetPosition() * direction + speed * dt);
}

void ComponentAsteroid::SetRandomValues()
{
	//direction = ...
	//speed = ...
}

void ComponentAsteroid::OnCollision()
{
	Entity* entity = GetEntity();
	ComponentCircleCollider* collider = (ComponentCircleCollider*)entity->GetComponent(Component::Type::CIRCLE_COLLIDER);

	Entity* entity2 = collider->GetCollision();
	ComponentAsteroid* asteroid = (ComponentAsteroid*)entity->GetComponent(Component::Type::ASTEROID);

	//// Maths
	//direction = asteroid->GetDirection() * asteroid->GetSpeed(); //not correct
	//speed = asteroid->GetSpeed(); //not correct
}
