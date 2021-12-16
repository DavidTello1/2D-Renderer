#include "ComponentCircleCollider.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleScene.h"
#include "ModulePhysics.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentSprite.h"
#include "ComponentRectCollider.h"

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

	Entity* entity = GetEntity();
	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	ComponentSprite* sprite = (ComponentSprite*)entity->GetComponent(Component::Type::SPRITE);
	if (sprite == nullptr)
		return;

	glm::vec2 size = sprite->GetSize();
	glm::vec2 pos = transform->GetPosition();
	center = glm::vec2(pos + radius + offset);
}

glm::vec2 ComponentCircleCollider::GetPosition()
{
	glm::vec2 pos = glm::vec2(0.0f);

	Entity* entity = GetEntity();
	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform != nullptr)
		pos = transform->GetPosition() + offset;

	return pos;
}
