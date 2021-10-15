#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

#define DEFAULT_MOVE_SPEED 5
#define DEFAULT_ZOOM_SPEED 1

class ComponentCamera : public Component
{
public:
	ComponentCamera(Entity* entity, glm::vec2 position = glm::vec2(0.0f), float zoom = 1.0f);
	~ComponentCamera() {};

	static inline Component::Type GetType() { return Component::Type::CAMERA; }

	const glm::vec2& GetPosition() const { return position; }
	void SetPosition(glm::vec2 pos) { position = pos; }

	const float& GetZoom() const { return zoom; }
	void SetZoom(float zoom_) { zoom = zoom_; }

	const float& GetZoomSpeed() const { return zoom_speed; }
	void SetZoomSpeed(float speed) { zoom_speed = speed; }

	const float& GetMoveSpeed() const { return move_speed; }
	void SetMoveSpeed(float speed) { move_speed = speed; }

	const glm::mat4& GetViewProjMatrix() const { return ViewProjMatrix; }
	void UpdateMatrices() { UpdateViewMatrix(); UpdateProjectionMatrix(); ViewProjMatrix = ViewMatrix * ProjectionMatrix; }

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	glm::vec2 position = glm::vec2(0.0f);
	float zoom = 0.0f;

	float move_speed = 0.0f;
	float zoom_speed = 0.0f;

	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

	glm::mat4 ViewProjMatrix = glm::mat4(1.0f);
};

