#include "ComponentRectCollider.h"

#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"

#include "Entity.h"
#include "ComponentTransform.h"

ComponentRectCollider::ComponentRectCollider(Entity* entity) :
	Component(Component::Type::RECT_COLLIDER, entity), position(0.0f), size(0.0f), offset(0.0f)
{
	App->physics->AddCollider(this);
}

ComponentRectCollider::~ComponentRectCollider()
{
	App->physics->RemoveCollider(this);
}

void ComponentRectCollider::OnUpdate(float dt)
{
	is_colliding = false;

	ComponentTransform* transform = (ComponentTransform*)GetEntity()->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	position = transform->GetPosition() + offset;
	size = transform->GetSize() * transform->GetScale();
}
