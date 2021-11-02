#include "ComponentAsteroid.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentCircleCollider.h"
#include "ComponentRectCollider.h" //

void ComponentAsteroid::OnUpdate(float dt)
{
	Entity* entity = GetEntity();

	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	ComponentRectCollider* collider = (ComponentRectCollider*)entity->GetComponent(Component::Type::RECT_COLLIDER); //***CHANGE TO CIRCLE
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
	ComponentRectCollider* collider = (ComponentRectCollider*)entity->GetComponent(Component::Type::RECT_COLLIDER); //***CHANGE TO CIRCLE

	Entity* entity2 = collider->GetCollision();
	ComponentAsteroid* asteroid = (ComponentAsteroid*)entity->GetComponent(Component::Type::ASTEROID); //***CHANGE TO CIRCLE

	//// Maths
	//direction = asteroid->GetDirection() * asteroid->GetSpeed(); //not correct
	//speed = asteroid->GetSpeed(); //not correct
}
