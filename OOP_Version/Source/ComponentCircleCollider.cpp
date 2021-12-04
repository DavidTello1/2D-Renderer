#include "ComponentCircleCollider.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentSprite.h"

void ComponentCircleCollider::OnUpdate(float dt)
{
	is_colliding = false;

	Entity* entity = GetEntity();
	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	ComponentSprite* sprite = (ComponentSprite*)entity->GetComponent(Component::Type::SPRITE);
	if (sprite == nullptr)
		return;

	glm::vec2 pos = transform->GetPosition();
	glm::vec2 size = sprite->GetSize();

	radius = size.x / 2;
	center = glm::vec2(pos.x + radius + offset.x, pos.y + radius + offset.y);

	if (GetCollision() != nullptr)
		is_colliding = true;
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

Entity* ComponentCircleCollider::GetCollision()
{
	return nullptr;
}