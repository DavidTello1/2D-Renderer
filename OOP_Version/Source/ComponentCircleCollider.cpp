#include "ComponentCircleCollider.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"

#include "Entity.h"
#include "ComponentTransform.h"
#include "ComponentSprite.h"

void ComponentCircleCollider::OnUpdate(float dt)
{
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
}

Entity* ComponentCircleCollider::GetCollision()
{
	return nullptr;
}

void ComponentCircleCollider::ShowCollider()
{
	glm::vec4 color;
	if (GetCollision() == nullptr)
		color = BLUE;
	else
		color = RED;
	color.a = 0.5f;

	App->renderer->DrawCircle(GetCenter(), GetRadius(), color);
}
