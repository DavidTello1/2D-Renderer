#include "S_Physics.h"

#include "Application.h"
#include "ModuleScene.h"

#include "Components.h"

#include "mmgr/mmgr.h"

void S_Physics::Init()
{
}

void S_Physics::Update(float dt)
{
	//--- Collision Detection
	std::vector<CollidingPair> colliding_pairs;
	for (EntityIdx entity1 : entities)
	{
		C_Collider& collider1 = App->scene->GetComponent<C_Collider>(entity1); // get first collider
		collider1.is_colliding = false;

		if (collider1.is_static)
			continue;

		for (EntityIdx entity2 : entities)
		{
			if (entity1 == entity2) // if same entity check next one
				continue;

			C_Collider& collider2 = App->scene->GetComponent<C_Collider>(entity2); // get second collider
			collider2.is_colliding = false;

			glm::vec2 distance = glm::vec2(0.0f);
			CollisionType type = CollisionType::ERROR;
			if (App->physics->CheckCollision(collider1, collider2, distance, type)) // check if they are colliding and retrieve distance & type
			{
				CollidingPair new_pair = { entity1, entity2, distance, type };

				// Check if pair already exists
				if (PairExists(new_pair, colliding_pairs) == false)
					colliding_pairs.push_back(new_pair); // add to vector
			}
		}
	}

	//--- Collision Resolution
	for (CollidingPair pair : colliding_pairs)
	{
		switch (pair.type) 
		{
		case CollisionType::CIRCLE_CIRCLE:
			CollisionCircleCircle(pair);
			break;

		case CollisionType::CIRCLE_RECT:
			CollisionCircleRect(pair);
			break;

		case CollisionType::RECT_RECT:
			CollisionRectRect(pair);
			break;

		case CollisionType::RECT_CIRCLE:
		{
			EntityIdx tmp = pair.entity1; // Changed order of entities for correct usage in collision resolution
			pair.entity1 = pair.entity2;
			pair.entity2 = tmp;
			CollisionCircleRect(pair);
			break;
		}
		case CollisionType::ERROR:
			break;
		}
	}

	//--- Movement Update
	for (EntityIdx entity : entities)
	{
		C_Transform& transform = App->scene->GetComponent<C_Transform>(entity);
		C_Collider& collider = App->scene->GetComponent<C_Collider>(entity);
		C_RigidBody rigidbody = App->scene->GetComponent<C_RigidBody>(entity);

		if (collider.is_static)
			continue;

		transform.position += rigidbody.velocity * dt;
		transform.rotation += rigidbody.rotation_speed * dt;
		
		collider.position = transform.position;
		collider.center = transform.position + collider.radius;
	}
}

// ---------------------------
// --- CIRCLE-RECT ---
void S_Physics::CollisionCircleRect(const CollidingPair pair)
{
	// Circle
	C_Collider& collider1 = App->scene->GetComponent<C_Collider>(pair.entity1);
	C_Transform& transform1 = App->scene->GetComponent<C_Transform>(pair.entity1);
	C_RigidBody& rigidbody1 = App->scene->GetComponent<C_RigidBody>(pair.entity1);

	// Rect
	C_Collider& collider2 = App->scene->GetComponent<C_Collider>(pair.entity2);
	C_Transform& transform2 = App->scene->GetComponent<C_Transform>(pair.entity2);
	C_RigidBody& rigidbody2 = App->scene->GetComponent<C_RigidBody>(pair.entity2);

	float overlap = collider1.radius - glm::sqrt(pair.distance.x * pair.distance.x + pair.distance.y * pair.distance.y); // Get Overlap
	if (std::isnan(overlap)) overlap = 0;
	if (overlap > 0)
	{
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(1.0f);

		switch (App->physics->GetCollisionDirection(pair.distance))
		{
		case CollisionDirection::UP:	{ position.y += overlap; velocity.y = -velocity.y; break; }
		case CollisionDirection::DOWN:	{ position.y -= overlap; velocity.y = -velocity.y; break; }
		case CollisionDirection::RIGHT: { position.x += overlap; velocity.x = -velocity.x; break; }
		case CollisionDirection::LEFT:	{ position.x -= overlap; velocity.x = -velocity.x; break; }

		case CollisionDirection::ERROR: 
			break;
		}

		// Circle
		collider1.is_colliding = true;
		if (collider1.is_static == false)
		{
			transform1.position += position;
			rigidbody1.velocity *= velocity;
			collider1.center = glm::vec2(transform1.position + collider1.radius);
		}

		// Rect
		collider2.is_colliding = true;
		if (collider2.is_static == false)
		{
			transform2.position -= position;
			rigidbody2.velocity *= velocity;
			collider2.position = transform2.position;
		}
	}
}

// --- CIRCLE-CIRCLE ---
void S_Physics::CollisionCircleCircle(const CollidingPair pair)
{
	C_Collider& collider1 = App->scene->GetComponent<C_Collider>(pair.entity1);
	C_Transform& transform1 = App->scene->GetComponent<C_Transform>(pair.entity1);
	C_RigidBody& rigidbody1 = App->scene->GetComponent<C_RigidBody>(pair.entity1);

	C_Collider& collider2 = App->scene->GetComponent<C_Collider>(pair.entity2);
	C_Transform& transform2 = App->scene->GetComponent<C_Transform>(pair.entity2);
	C_RigidBody& rigidbody2 = App->scene->GetComponent<C_RigidBody>(pair.entity2);

	// --- Static Collision
	float dist = glm::sqrt(pair.distance.x * pair.distance.x + pair.distance.y * pair.distance.y);
	float overlap = 0.5f * (dist - collider1.radius - collider2.radius);

	collider1.is_colliding = true;
	if (collider1.is_static == false)
	{
		collider1.center -= overlap * pair.distance / dist;
		transform1.position = glm::vec2(collider1.center - collider1.radius);
	}

	collider2.is_colliding = true;
	if (collider2.is_static == false)
	{
		collider2.center += overlap * pair.distance / dist;
		transform2.position = glm::vec2(collider2.center - collider2.radius);
	}

	// --- Dynamic Collision
	float fDistance = sqrtf((collider1.center.x - collider2.center.x) * (collider1.center.x - collider2.center.x) + 
							(collider1.center.y - collider2.center.y) * (collider1.center.y - collider2.center.y));

	glm::vec2 normal = glm::vec2((collider2.center - collider1.center) / fDistance);
	glm::vec2 tangent = glm::vec2(-normal.y, normal.x);

	// Dot Product Tangent
	float dpTan1 = rigidbody1.velocity.x * tangent.x + rigidbody1.velocity.y * tangent.y;
	float dpTan2 = rigidbody2.velocity.x * tangent.x + rigidbody2.velocity.y * tangent.y;

	// Dot Product Normal
	float dpNorm1 = rigidbody1.velocity.x * normal.x + rigidbody1.velocity.y * normal.y;
	float dpNorm2 = rigidbody2.velocity.x * normal.x + rigidbody2.velocity.y * normal.y;

	float m1 = (dpNorm1 * (rigidbody1.mass - rigidbody2.mass) + 2.0f * rigidbody2.mass * dpNorm2) / (rigidbody1.mass + rigidbody2.mass);
	float m2 = (dpNorm2 * (rigidbody2.mass - rigidbody1.mass) + 2.0f * rigidbody1.mass * dpNorm1) / (rigidbody1.mass + rigidbody2.mass);

	// Update RigidBody Velocities
	rigidbody1.velocity = tangent * dpTan1 + normal * m1;
	rigidbody2.velocity = tangent * dpTan2 + normal * m2;
}

// --- RECT-RECT ---
void S_Physics::CollisionRectRect(const CollidingPair pair)
{
	C_Collider& collider1 = App->scene->GetComponent<C_Collider>(pair.entity1);
	C_Transform& transform1 = App->scene->GetComponent<C_Transform>(pair.entity1);
	C_RigidBody& rigidbody1 = App->scene->GetComponent<C_RigidBody>(pair.entity1);

	C_Collider& collider2 = App->scene->GetComponent<C_Collider>(pair.entity2);
	C_Transform& transform2 = App->scene->GetComponent<C_Transform>(pair.entity2);
	C_RigidBody& rigidbody2 = App->scene->GetComponent<C_RigidBody>(pair.entity2);

	//*** right now does nothing - update code
}

// ---------------------------
// ---------------------------
bool S_Physics::PairExists(const CollidingPair& new_pair, const std::vector<CollidingPair>& list) const
{
	for (CollidingPair pair : list)
	{
		if ((pair.entity1 == new_pair.entity1 && pair.entity2 == new_pair.entity2) ||
			(pair.entity1 == new_pair.entity2 && pair.entity2 == new_pair.entity1))
		{
			return true;
		}
	}
	return false;
}