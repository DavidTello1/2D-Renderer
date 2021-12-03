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

	center = glm::vec2(pos.x + size.x / 2 + offset.x, pos.y + size.y / 2 + offset.y);

	if (GetCollision() != nullptr)
		is_colliding = true;
}

Entity* ComponentCircleCollider::GetCollision()
{
	return nullptr;
}