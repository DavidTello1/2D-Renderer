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

struct C_Camera {
	glm::mat4 projection;
};

struct C_CameraController {
	float zoom;

	float move_speed;
	float zoom_speed;
};

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
