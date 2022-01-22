#pragma once
#include "Globals.h"

#include "glm/include/glm/glm.hpp"

struct C_Transform {
	glm::vec2 position;
	glm::vec2 scale;
	float rotation;

	glm::vec2 size;
};

struct C_Sprite {
	uint shader;
	uint texture;
};

struct C_Renderer {
	bool render;
};

// --- CAMERA ---
struct C_Camera {
	glm::vec3 position;
	float rotation;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewProjMatrix;
};

struct C_CameraController {
	float zoom;

	float move_speed;
	float zoom_speed;

	//ComponentCamera* camera = nullptr;
};

// --- COLLIDERS ---
struct C_RigidBody {
	glm::vec2 velocity;
	float mass;
	float rotation_speed;
};

struct C_RectCollider {
	bool is_colliding;
	bool is_static;

	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 offset;
};

struct C_CircleCollider {
	bool is_colliding;
	bool is_static;

	glm::vec2 center;
	float radius;
	glm::vec2 offset;
};
