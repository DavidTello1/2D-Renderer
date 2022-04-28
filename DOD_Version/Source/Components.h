#pragma once
#include "Globals.h"

#include "glm/include/glm/glm.hpp"

enum class ComponentType {
	C_TRANSFORM = 0,
	C_SPRITE,
	C_RENDERER,
	C_CAMERA,
	C_CAMERACONTROLLER,
	C_RIGIDBODY,
	C_COLLIDER,
	TYPE_COUNT
};

struct C_Transform 
{
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 scale = glm::vec2(0.0f);
	float rotation = 0.0f;

	glm::vec2 size = glm::vec2(0.0f);
};

struct C_Sprite 
{
	uint shader = 0;
	uint texture = 0;
};

struct C_Renderer 
{
	bool render = false;
};

struct C_Camera 
{
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 viewproj = glm::mat4(1.0f);
};

struct C_CameraController 
{
	float zoom = 0.0f;

	float move_speed = 0.0f;
	float zoom_speed = 0.0f;
};

struct C_RigidBody 
{
	glm::vec2 velocity = glm::vec2(0.0f);
	float mass = 0.0f;
	float rotation_speed = 0.0f;
};

struct C_Collider 
{
	enum class ColliderType {
		UNKNOWN = 0,
		RECT,
		CIRCLE
	};

	bool is_static = false;
	bool is_colliding = false;

	ColliderType type = ColliderType::UNKNOWN;
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(0.0f);
	glm::vec2 center = glm::vec2(0.0f);
	float radius = 0.0f;
};