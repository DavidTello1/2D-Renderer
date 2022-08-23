#include "S_Physics.h"

#include "Application.h"
#include "ModuleScene.h"

#include "Components.h"

#include "Optick/include/optick.h"
#include "mmgr/mmgr.h"

S_Physics::S_Physics()
{
	this->mask.set(App->scene->GetComponentType<C_Transform>());
	this->mask.set(App->scene->GetComponentType<C_RigidBody>());
	this->mask.set(App->scene->GetComponentType<C_Collider>());
}

S_Physics::~S_Physics()
{
}

void S_Physics::Update(float dt)
{
	OPTICK_PUSH("Get Components");
	//--- Get Components ---
	transforms.clear();
	rigidbodies.clear();
	colliders.clear();
	for (EntityIdx entity : entities)
	{
		transforms.push_back(App->scene->GetComponent<C_Transform>(entity));
		rigidbodies.push_back(App->scene->GetComponent<C_RigidBody>(entity));
		colliders.push_back(App->scene->GetComponent<C_Collider>(entity));
	}
	OPTICK_POP();

	OPTICK_PUSH("Collision Detection");
	//--- Collision Detection
	std::vector<CollidingPair> colliding_pairs;
	
	for (size_t i = 0; i < entities.size(); ++i)
	{
		C_Collider collider1 = colliders[i]; // get first collider
		collider1.is_colliding = false;

		if (collider1.is_static) // if collider is static check next one
			continue;

		for (size_t j = 0; j < entities.size(); ++j)
		{
			if (i == j) // if same entity check next one
				continue;

			C_Collider& collider2 = colliders[j]; // get second collider
			collider2.is_colliding = false;

			glm::vec2 distance = glm::vec2(0.0f);
			CollisionType type = CollisionType::ERROR;
			if (CheckCollision(collider1, collider2, distance, type)) // check if they are colliding and retrieve distance & type
			{
				CollidingPair new_pair = { i, j, distance, type };

				// Check if pair already exists
				if (PairExists(new_pair, colliding_pairs) == false)
					colliding_pairs.push_back(new_pair); // add to vector
			}
		}
	}
	OPTICK_POP();

	OPTICK_PUSH("Collision Resolution");
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
	OPTICK_POP();

	OPTICK_PUSH("Movement Update");
	//--- Movement Update
	for (size_t i = 0; i < entities.size(); ++i)
	{
		C_Transform& transform = transforms[i];
		C_Collider& collider = colliders[i];
		C_RigidBody rigidbody = rigidbodies[i];

		if (collider.is_static)
			continue;

		transform.position += rigidbody.velocity * dt;
		transform.rotation += rigidbody.rotation_speed * dt;
		
		collider.position = transform.position;
		collider.center = transform.position + collider.radius;
	}
	OPTICK_POP();

	// --- Set Components ---
	OPTICK_PUSH("Set Components");
	for (size_t i = 0; i < entities.size(); ++i)
	{
		App->scene->SetComponent<C_Transform>(entities[i], transforms[i]);
		App->scene->SetComponent<C_RigidBody>(entities[i], rigidbodies[i]);
		App->scene->SetComponent<C_Collider>(entities[i], colliders[i]);
	}
	OPTICK_POP();
}

// -----------------
// --- COLLISION DETECTION ---
bool S_Physics::CheckCollision(const C_Collider& collider1, const C_Collider& collider2, glm::vec2& distance, CollisionType& type)
{
	switch (collider1.type)
	{
		// --- CIRCLE ---
	case C_Collider::ColliderType::CIRCLE:
	{
		switch (collider2.type)
		{
		case C_Collider::ColliderType::CIRCLE: // Circle - Circle
		{
			type = CollisionType::CIRCLE_CIRCLE;
			distance = collider1.center - collider2.center;
			return ((distance.x * distance.x) + (distance.y * distance.y) < (collider1.radius + collider2.radius) * (collider1.radius + collider2.radius));
		}
		case C_Collider::ColliderType::RECT: // Circle - Rect
		{
			type = CollisionType::CIRCLE_RECT;
			distance = collider1.center - glm::clamp(collider1.center, collider2.position, collider2.position + collider2.size);
			return ((distance.x * distance.x) + (distance.y * distance.y) <= (collider1.radius * collider1.radius));
		}
		case C_Collider::ColliderType::UNKNOWN: {
			LOG("---Error: Collider Type Unknown");
			type = CollisionType::ERROR;
			return false;
		}
		}
	}

	// --- RECT ---
	case C_Collider::ColliderType::RECT:
	{
		switch (collider2.type)
		{
		case C_Collider::ColliderType::CIRCLE: // Rect - Circle
		{
			type = CollisionType::RECT_CIRCLE;
			distance = collider2.center - glm::clamp(collider2.center, collider1.position, collider1.position + collider1.size);
			return ((distance.x * distance.x) + (distance.y * distance.y) < (collider2.radius * collider2.radius));
		}
		case C_Collider::ColliderType::RECT: // Rect - Rect
		{
			type = CollisionType::RECT_RECT;
			distance = glm::vec2(0.0f); //*** GET ACTUAL VALUE
			return (collider1.position.x < collider2.position.x + collider2.size.x &&
				collider1.position.x + collider1.size.x > collider2.position.x &&
				collider1.position.y < collider2.position.y + collider2.size.y &&
				collider1.position.y + collider1.size.y > collider2.position.y);
		}
		case C_Collider::ColliderType::UNKNOWN: {
			LOG("---Error: Collider Type Unknown");
			type = CollisionType::ERROR;
			return false; }
		}
	}

	case C_Collider::ColliderType::UNKNOWN:
	{
		LOG("---Error: Collider Type Unknown");
		type = CollisionType::ERROR;
		return false;
	}
	}

	return false;
}

// -------------------
// --- COLLISION RESOLUTION ---
// --- CIRCLE-RECT ---
void S_Physics::CollisionCircleRect(const CollidingPair pair)
{
	// Circle
	C_Collider& collider1 = colliders[pair.entity1];
	C_Transform& transform1 = transforms[pair.entity1];
	C_RigidBody& rigidbody1 = rigidbodies[pair.entity1];

	// Rect
	C_Collider& collider2 = colliders[pair.entity2];
	C_Transform& transform2 = transforms[pair.entity2];
	C_RigidBody& rigidbody2 = rigidbodies[pair.entity2];

	float overlap = collider1.radius - glm::sqrt(pair.distance.x * pair.distance.x + pair.distance.y * pair.distance.y); // Get Overlap
	if (std::isnan(overlap)) overlap = 0;
	if (overlap > 0)
	{
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(1.0f);

		switch (GetCollisionDirection(pair.distance))
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
	C_Collider& collider1 = colliders[pair.entity1];
	C_Transform& transform1 = transforms[pair.entity1];
	C_RigidBody& rigidbody1 = rigidbodies[pair.entity1];

	C_Collider& collider2 = colliders[pair.entity2];
	C_Transform& transform2 = transforms[pair.entity2];
	C_RigidBody& rigidbody2 = rigidbodies[pair.entity2];

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
	C_Collider& collider1 = colliders[pair.entity1];
	C_Transform& transform1 = transforms[pair.entity1];
	C_RigidBody& rigidbody1 = rigidbodies[pair.entity1];

	C_Collider& collider2 = colliders[pair.entity2];
	C_Transform& transform2 = transforms[pair.entity2];
	C_RigidBody& rigidbody2 = rigidbodies[pair.entity2];

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

CollisionDirection S_Physics::GetCollisionDirection(glm::vec2 distance) const
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};

	float max = 0.0f;
	uint direction = -1;
	for (uint i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(distance), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			direction = i;
		}
	}
	return (CollisionDirection)direction;
}
