#include "ComponentRectCollider.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentSprite.h";

void ComponentRectCollider::OnUpdate(float dt)
{
	is_colliding = false;

	Entity* entity = GetEntity();
	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	ComponentSprite* sprite = (ComponentSprite*)entity->GetComponent(Component::Type::SPRITE);
	if (sprite == nullptr)
		return;

	position = transform->GetPosition() + offset;

	if (GetCollision() != nullptr)
		is_colliding = true;
}

Entity* ComponentRectCollider::GetCollision()
{
	return nullptr;
}

void ComponentRectCollider::ShowCollider()
{
	glm::vec4 color;
	if (GetCollision() == nullptr)
		color = BLUE;
	else
		color = RED;
	color.a = 0.5f;

	App->renderer->DrawQuad(GetPosition(), GetSize(), color);
}
