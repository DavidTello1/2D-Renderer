#include "ComponentCircleCollider.h"

#include "Application.h"
#include "ModulePhysics.h"

#include "Entity.h"
#include "ComponentTransform.h"

ComponentCircleCollider::ComponentCircleCollider(Entity* entity) :
	Component(Component::Type::CIRCLE_COLLIDER, entity), center(0.0f), radius(0.0f), offset(0.0f)
{
	App->physics->AddCollider(this);
	collision.Reset();
}

ComponentCircleCollider::~ComponentCircleCollider()
{
	App->physics->RemoveCollider(this);
}

void ComponentCircleCollider::OnUpdate(float dt)
{
	collision.Reset();

	ComponentTransform* transform = (ComponentTransform*)GetEntity()->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	center = glm::vec2(transform->GetPosition() + radius + offset);
}

glm::vec2 ComponentCircleCollider::GetPosition()
{
	glm::vec2 pos = glm::vec2(0.0f);

	ComponentTransform* transform = (ComponentTransform*)GetEntity()->GetComponent(Component::Type::TRANSFORM);
	if (transform != nullptr)
		pos = transform->GetPosition() + offset;

	return pos;
}
