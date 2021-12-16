#include "ModulePhysics.h"

#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer.h"
#include "ModuleResources.h"

#include "Entity.h"
#include "ComponentRectCollider.h"
#include "ComponentCircleCollider.h"
#include "ComponentTransform.h"
#include "ComponentAsteroid.h"

bool ModulePhysics::Init()
{
	return true;
}

bool ModulePhysics::Start()
{
	return true;
}

bool ModulePhysics::Update(float dt)
{
	// --- Check for all possible collisions ---
	for (ComponentRectCollider* rect1 : rect_colliders)
	{
		rect1->DoCollisions();
	}

	for (ComponentCircleCollider* circle1 : circle_colliders)
	{
		circle1->DoCollisions();

		// Update Asteroid
		ComponentAsteroid* asteroid = (ComponentAsteroid*)circle1->GetEntity()->GetComponent(Component::Type::ASTEROID);
		if (asteroid != nullptr)
			asteroid->Move(dt);
	}

	return true;
}

bool ModulePhysics::CleanUp()
{
	for (size_t i = 0, size = rect_colliders.size(); i < size; ++i)
	{
		RELEASE(rect_colliders[i]);
		rect_colliders.erase(rect_colliders.begin() + i);
		break;
	}
	rect_colliders.clear();

	for (size_t i = 0, size = circle_colliders.size(); i < size; ++i)
	{
		RELEASE(circle_colliders[i]);
		circle_colliders.erase(circle_colliders.begin() + i);
		break;
	}
	circle_colliders.clear();

	return true;
}

void ModulePhysics::DrawColliders()
{
	// Rect Colliders
	for (ComponentRectCollider* rect : rect_colliders)
	{
		ComponentTransform* transform = (ComponentTransform*)rect->GetEntity()->GetComponent(Component::Type::TRANSFORM);
		if (transform == nullptr)
			continue;

		glm::vec4 color;
		if (rect->IsColliding())
			color = RED;
		else
			color = BLUE;
		color.a = 0.5f;

		App->renderer->DrawQuad(App->resources->default_shader, rect->GetPosition(), rect->GetSize() * transform->GetScale(),
			App->resources->default_tex, color);
	}

	// Circle Colliders
	for (ComponentCircleCollider* circle : circle_colliders)
	{
		ComponentTransform* transform = (ComponentTransform*)circle->GetEntity()->GetComponent(Component::Type::TRANSFORM);
		if (transform == nullptr)
			continue;

		glm::vec4 color;
		if (circle->IsColliding())
			color = RED;
		else
			color = BLUE;
		color.a = 0.5f;

		App->renderer->DrawCircle(App->resources->default_shader, circle->GetPosition(), circle->GetRadius() * 2 * transform->GetScale().x, color);
	}
}

//--------------------------------------
// --- UTILS ---
void ModulePhysics::AddCollider(ComponentRectCollider* collider)
{
	if (Exists(collider) == -1)
		rect_colliders.push_back(collider);
}

void ModulePhysics::AddCollider(ComponentCircleCollider* collider)
{
	if (Exists(collider) == -1)
		circle_colliders.push_back(collider);
}

void ModulePhysics::RemoveCollider(ComponentRectCollider* collider)
{
	int index = Exists(collider);

	if (index != -1)
		rect_colliders.erase(rect_colliders.begin() + index);
}

void ModulePhysics::RemoveCollider(ComponentCircleCollider* collider)
{
	int index = Exists(collider);

	if (index != -1)
		circle_colliders.erase(circle_colliders.begin() + index);
}

int ModulePhysics::Exists(ComponentRectCollider* collider)
{
	for (size_t i = 0, size = rect_colliders.size(); i < size; ++i)
	{
		if (rect_colliders[i] == collider)
			return i;
	}

	return -1;
}

int ModulePhysics::Exists(ComponentCircleCollider* collider)
{
	for (size_t i = 0, size = circle_colliders.size(); i < size; ++i)
	{
		if (circle_colliders[i] == collider)
			return i;
	}

	return -1;
}

////--------------------------------------
//// --- COLLISION DETECTION ---
//bool ModulePhysics::CheckCollision(ComponentRectCollider* collider1, ComponentRectCollider* collider2) // Rect - Rect
//{
//	Collision collision1;
//	collision1.Reset();
//	glm::vec2 pos1 = collider1->GetPosition();
//	glm::vec2 size1 = collider1->GetSize();
//
//	Collision collision2;
//	collision2.Reset();
//	glm::vec2 pos2 = collider2->GetPosition();
//	glm::vec2 size2 = collider2->GetSize();
//
//	//***GET DISTANCE
//
//	if (pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x && 
//		pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y)
//	{
//		collision1.has_collided = true;
//		//collision1.distance = dist;
//		collision1.type = collider2->GetType();
//		collider1->SetCollision(collision1);
//
//		collision2.has_collided = true;
//		//collision2.distance = dist;
//		collision2.type = collider1->GetType();
//		collider2->SetCollision(collision2);
//
//		return true;
//	}
//	return false;
//}
//
//bool ModulePhysics::CheckCollision(ComponentCircleCollider* collider1, ComponentCircleCollider* collider2) // Circle - Circle
//{
//	Collision collision1;
//	collision1.Reset();
//	glm::vec2 center1 = collider1->GetCenter();
//	float radius1 = collider1->GetRadius();
//
//	Collision collision2;
//	collision2.Reset();
//	glm::vec2 center2 = collider2->GetCenter();
//	float radius2 = collider2->GetRadius();
//
//
//	glm::vec2 dist = center1 - center2;
//	float distance = glm::sqrt(dist.x * dist.x + dist.y * dist.y);
//
//	if (distance < radius1 + radius2)
//	{
//		collision1.has_collided = true;
//		collision1.distance = dist;
//		collision1.type = collider2->GetType();
//		collision1.other_radius = collider2->GetRadius();
//		collider1->SetCollision(collision1);
//
//		collision2.has_collided = true;
//		collision2.distance = dist;
//		collision2.type = collider1->GetType();
//		collision2.other_radius = collider1->GetRadius();
//		collider2->SetCollision(collision2);
//
//		return true;
//	}
//	return false;
//}
//
//bool ModulePhysics::CheckCollision(ComponentCircleCollider* collider1, ComponentRectCollider* collider2) // Circle - Rect
//{
//	Collision collision1;
//	collision1.Reset();
//	glm::vec2 center = collider1->GetCenter();
//	float radius = collider1->GetRadius();
//
//	Collision collision2;
//	collision2.Reset();
//	glm::vec2 pos = collider2->GetPosition();
//	glm::vec2 size = collider2->GetSize();
//
//
//	glm::vec2 dist = center - glm::clamp(center, pos, pos + size);
//	float distanceSquared = (dist.x * dist.x) + (dist.y * dist.y);
//
//	if (distanceSquared < (radius * radius))
//	{
//		collision1.has_collided = true;
//		collision1.distance = dist;
//		collision1.type = collider2->GetType();
//		collider1->SetCollision(collision1);
//
//		collision2.has_collided = true;
//		collision2.distance = dist;
//		collision2.type = collider1->GetType();
//		collider2->SetCollision(collision2);
//
//		return true;
//	}
//	return false;
//}
