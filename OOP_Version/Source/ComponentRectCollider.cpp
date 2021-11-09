#include "ComponentRectCollider.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleScene.h"

#include "Entity.h"
#include "ComponentTransform.h"

void ComponentRectCollider::OnUpdate(float dt)
{
	is_colliding = false;

	Entity* entity = GetEntity();
	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	position = transform->GetPosition() + offset;

	if (GetCollision() != nullptr)
		is_colliding = true;
}

Entity* ComponentRectCollider::GetCollision()
{
	return nullptr;
}
