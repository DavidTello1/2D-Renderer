#include "ModulePhysics.h"

#include "Application.h"
#include "ModuleGame.h"
#include "ModuleRenderer.h"
#include "ModuleResources.h"

#include "EntityManager.h"

#include "Optick/include/optick.h"
#include "mmgr/mmgr.h"

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
	OPTICK_CATEGORY("Physics Update", Optick::Category::Physics);

	colliding_pairs.clear(); //clear list

	OPTICK_PUSH("Static Collisions");
	for (ComponentCircleCollider* collider1 : circle_colliders)
	{
		OPTICK_PUSH("Circle-Circle Collisions");
		for (ComponentCircleCollider* collider2 : circle_colliders) // Circle-Circle Collisions
		{
			if (collider1 == collider2)
				continue;

			Collision collision = CheckCollision(collider1, collider2);
			if (collision.has_collided)
			{
				ResolveCollisionStatic(collision.distance, collider1, collider2); // Resolve static collision (overlapping)

				// Add new pair to list
				CollidingPairs new_pair;
				new_pair.collider1 = collider1;
				new_pair.collider2 = collider2;
				new_pair.distance = collision.distance;
				if (FindPair(new_pair, colliding_pairs) == false) 
					colliding_pairs.push_back(new_pair);

				break;
			}
		}
		OPTICK_POP();

		OPTICK_PUSH("Circle-Rect Collisions");
		for (ComponentRectCollider* collider2 : rect_colliders) // Circle-Rect Collisions
		{
			Collision collision = CheckCollision(collider1, collider2);
			if (collision.has_collided)
			{
				ResolveCollision(collision.distance, collider1, collider2);
				break;
			}
		}	
		OPTICK_POP();
	}
	OPTICK_POP();

	// --- Solve Colliding Pairs
	OPTICK_PUSH("Dynamic Collision");
	for (CollidingPairs pair : colliding_pairs)
	{
		if (FindPair(pair, prev_colliding_pairs) == false) // Check if pair had collided in previous frame, if yes do not resolve again
			ResolveCollisionDynamic(pair.distance, pair.collider1, pair.collider2);
	}
	prev_colliding_pairs = colliding_pairs; //set previous list to new list
	OPTICK_POP();

	//// --- Update Asteroids ---
	//OPTICK_PUSH("Update Asteroids");
	//for (ComponentCircleCollider* collider1 : circle_colliders)
	//{
	//	ComponentAsteroid* asteroid = (ComponentAsteroid*)collider1->GetEntity()->GetComponent(Component::Type::ASTEROID);
	//	if (asteroid != nullptr)
	//		asteroid->Move(dt);
	//}
	//OPTICK_POP();

	return true;
}

bool ModulePhysics::CleanUp()
{
	//for (size_t i = 0, size = rect_colliders.size(); i < size; ++i)
	//{
	//	RELEASE(rect_colliders[i]);
	//	rect_colliders.erase(rect_colliders.begin() + i);
	//}
	//rect_colliders.clear();

	//for (size_t i = 0, size = circle_colliders.size(); i < size; ++i)
	//{
	//	RELEASE(circle_colliders[i]);
	//	circle_colliders.erase(circle_colliders.begin() + i);
	//}
	//circle_colliders.clear();

	return true;
}

void ModulePhysics::DrawColliders()
{
	//// Rect Colliders
	//for (ComponentRectCollider* rect : rect_colliders)
	//{
	//	ComponentTransform* transform = (ComponentTransform*)rect->GetEntity()->GetComponent(Component::Type::TRANSFORM);
	//	if (transform == nullptr)
	//		continue;

	//	glm::vec4 color;
	//	if (rect->IsColliding())
	//		color = RED;
	//	else
	//		color = BLUE;
	//	color.a = 0.5f;

	//	App->renderer->DrawQuad(App->resources->default_shader, rect->GetPosition(), rect->GetSize() * transform->GetScale(),
	//		App->resources->default_tex, color);
	//}

	//// Circle Colliders
	//for (ComponentCircleCollider* circle : circle_colliders)
	//{
	//	ComponentTransform* transform = (ComponentTransform*)circle->GetEntity()->GetComponent(Component::Type::TRANSFORM);
	//	if (transform == nullptr)
	//		continue;

	//	glm::vec4 color;
	//	if (circle->IsColliding())
	//		color = RED;
	//	else
	//		color = BLUE;
	//	color.a = 0.5f;

	//	App->renderer->DrawCircle(App->resources->default_shader, circle->GetPosition(), circle->GetRadius() * 2 * transform->GetScale().x, color);
	//}
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
	for (int i = 0, size = rect_colliders.size(); i < size; ++i)
	{
		if (rect_colliders[i] == collider)
			return i;
	}

	return -1;
}

int ModulePhysics::Exists(ComponentCircleCollider* collider)
{
	for (int i = 0, size = circle_colliders.size(); i < size; ++i)
	{
		if (circle_colliders[i] == collider)
			return i;
	}

	return -1;
}

bool ModulePhysics::FindPair(CollidingPairs new_pair, std::vector<CollidingPairs> list)
{
	for (CollidingPairs pair : list)
	{
		if ((pair.collider1 == new_pair.collider1 && pair.collider2 == new_pair.collider2) ||
			(pair.collider1 == new_pair.collider2 && pair.collider2 == new_pair.collider1))
		{
			return true;
		}
	}

	return false;
}

//--------------------------------------
// --- COLLISION DETECTION ---
const int& ModulePhysics::GetCollisionDirection(glm::vec2 distance) const
{
	float max = 0.0f;
	int direction = -1;

	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};

	for (uint i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(distance), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			direction = i;
		}
	}
	return direction;
}

Collision ModulePhysics::CheckCollision(ComponentRectCollider* collider1, ComponentRectCollider* collider2) // Rect - Rect
{
	Collision collision;
	//glm::vec2 pos1 = collider1->GetPosition();
	//glm::vec2 size1 = collider1->GetSize();

	//glm::vec2 pos2 = collider2->GetPosition();
	//glm::vec2 size2 = collider2->GetSize();


	//collision.distance = glm::vec2(0.0f); //***GET ACTUAL DISTANCE

	//if (pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x && 
	//	pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y)
	//{
	//	collider1->SetColliding(true);
	//	collision.has_collided = true;
	//}
	//else
	//	collision.has_collided = false;

	return collision;
}

Collision ModulePhysics::CheckCollision(ComponentRectCollider* collider1, ComponentCircleCollider* collider2) // Rect - Circle
{
	Collision collision;
	//glm::vec2 pos = collider1->GetPosition();
	//glm::vec2 size = collider1->GetSize();

	//glm::vec2 center = collider2->GetCenter();
	//float radius = collider2->GetRadius();


	//collision.distance = center - glm::clamp(center, pos, pos + size);
	//float distanceSquared = (collision.distance.x * collision.distance.x) + (collision.distance.y * collision.distance.y);

	//if (distanceSquared < (radius * radius))
	//{
	//	collider1->SetColliding(true);
	//	collision.has_collided = true;
	//}
	//else
	//	collision.has_collided = false;

	return collision;
}

Collision ModulePhysics::CheckCollision(ComponentCircleCollider* collider1, ComponentCircleCollider* collider2) // Circle - Circle
{
	Collision collision;
	//glm::vec2 center1 = collider1->GetCenter();
	//float radius1 = collider1->GetRadius();

	//glm::vec2 center2 = collider2->GetCenter();
	//float radius2 = collider2->GetRadius();


	//collision.distance = center1 - center2;
	//float dist = glm::sqrt(collision.distance.x * collision.distance.x + collision.distance.y * collision.distance.y);

	//if (dist < radius1 + radius2)
	//{
	//	collider1->SetColliding(true);
	//	collision.has_collided = true;
	//}
	//else
	//	collision.has_collided = false;

	return collision;
}

Collision ModulePhysics::CheckCollision(ComponentCircleCollider* collider1, ComponentRectCollider* collider2) // Circle - Rect
{
	Collision collision;
	//glm::vec2 center = collider1->GetCenter();
	//float radius = collider1->GetRadius();

	//glm::vec2 pos = collider2->GetPosition();
	//glm::vec2 size = collider2->GetSize();


	//collision.distance = center - glm::clamp(center, pos, pos + size);
	//float distanceSquared = (collision.distance.x * collision.distance.x) + (collision.distance.y * collision.distance.y);

	//if (distanceSquared < (radius * radius))
	//{
	//	collider1->SetColliding(true);
	//	collision.has_collided = true;
	//}
	//else
	//	collision.has_collided = false;

	return collision;
}

//--------------------------------------
// --- COLLISION RESOULTION ---
void ModulePhysics::ResolveCollisionStatic(glm::vec2 distance, ComponentCircleCollider* collider1, ComponentCircleCollider* collider2) // Circle - Circle
{
	//ComponentTransform* transform1 = (ComponentTransform*)collider1->GetEntity()->GetComponent(Component::Type::TRANSFORM);
	//if (transform1 == nullptr)
	//	return;

	//ComponentTransform* transform2 = (ComponentTransform*)collider1->GetEntity()->GetComponent(Component::Type::TRANSFORM);
	//if (transform2 == nullptr)
	//	return;

	//float dist = glm::sqrt(distance.x * distance.x + distance.y * distance.y);
	//float overlap = 0.5f * (dist - collider1->GetRadius() - collider2->GetRadius());

	//// Displace Current Ball away from collision
	//glm::vec2 center = collider1->GetCenter();
	//center -= overlap * distance / dist;
	//collider1->SetCenter(center);

	//// Update Transform Position
	//glm::vec2 final_pos = glm::vec2(center - collider1->GetRadius() - collider1->GetOffset());
	//transform1->SetPosition(final_pos);
}

void ModulePhysics::ResolveCollisionDynamic(glm::vec2 distance, ComponentCircleCollider* collider1, ComponentCircleCollider* collider2)
{
	//// Update Asteroid Velocity
	//ComponentAsteroid* asteroid1 = (ComponentAsteroid*)collider1->GetEntity()->GetComponent(Component::Type::ASTEROID);
	//ComponentAsteroid* asteroid2 = (ComponentAsteroid*)collider2->GetEntity()->GetComponent(Component::Type::ASTEROID);
	//if (asteroid1 == nullptr || asteroid2 == nullptr)
	//	return;

	//glm::vec2 pos1 = collider1->GetPosition();
	//glm::vec2 pos2 = collider2->GetPosition();
	//glm::vec2 vel1 = asteroid1->GetVelocity();
	//glm::vec2 vel2 = asteroid2->GetVelocity();
	//float mass1 = asteroid1->GetMass();
	//float mass2 = asteroid2->GetMass();
	//float fDistance = sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y));

	//// Normal
	//glm::vec2 normal;
	//normal.x = (pos2.x - pos1.x) / fDistance;
	//normal.y = (pos2.y - pos1.y) / fDistance;

	//// Tangent
	//glm::vec2 tangent;
	//tangent.x = -normal.y;
	//tangent.y = normal.x;

	//// Dot Product Tangent
	//float dpTan1 = vel1.x * tangent.x + vel1.y * tangent.y;
	//float dpTan2 = vel2.x * tangent.x + vel2.y * tangent.y;

	//// Dot Product Normal
	//float dpNorm1 = vel1.x * normal.x + vel1.y * normal.y;
	//float dpNorm2 = vel2.x * normal.x + vel2.y * normal.y;

	//float m1 = (dpNorm1 * (mass1 - mass2) + 2.0f * mass2 * dpNorm2) / (mass1 + mass2);
	//float m2 = (dpNorm2 * (mass2 - mass1) + 2.0f * mass1 * dpNorm1) / (mass1 + mass2);

	//// Update Asteroid Velocities
	//vel1 = tangent * dpTan1 + normal * m1;
	//vel2 = tangent * dpTan2 + normal * m2;
	//asteroid1->SetVelocity(vel1);
	//asteroid2->SetVelocity(vel2);
}

void ModulePhysics::ResolveCollision(glm::vec2 distance, ComponentCircleCollider* collider1, ComponentRectCollider* collider2) // Circle - Rect
{
	//ComponentTransform* transform = (ComponentTransform*)collider1->GetEntity()->GetComponent(Component::Type::TRANSFORM);
	//if (transform == nullptr)
	//	return;

	//float dist = glm::sqrt(distance.x * distance.x + distance.y * distance.y);
	//float overlap = collider1->GetRadius() - dist; // Get Overlap
	//if (std::isnan(overlap)) overlap = 0;

	//if (overlap > 0)
	//{
	//	int direction = GetCollisionDirection(distance);
	//	glm::vec2 pos = transform->GetPosition();
	//	glm::vec2 velocity = glm::vec2(1.0f);

	//	if (direction == 0) // UP
	//	{
	//		velocity.y = -velocity.y;
	//		pos.y += overlap;
	//	}
	//	else if (direction == 1) // RIGHT
	//	{
	//		velocity.x = -velocity.x;
	//		pos.x += overlap;
	//	}
	//	else if (direction == 3) // LEFT
	//	{
	//		velocity.x = -velocity.x;
	//		pos.x -= overlap;
	//	}
	//	else if (direction == 2)// DOWN
	//	{
	//		velocity.y = -velocity.y;
	//		pos.y -= overlap;
	//	}

	//	// Update Transform Position
	//	transform->SetPosition(pos);

	//	// Update Asteroid Velocity
	//	ComponentAsteroid* asteroid = (ComponentAsteroid*)collider1->GetEntity()->GetComponent(Component::Type::ASTEROID);
	//	if (asteroid != nullptr)
	//		asteroid->SetVelocity(asteroid->GetVelocity() * velocity);
	//}
}
