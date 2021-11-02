#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

#define DEFAULT_MOVE_SPEED 100
#define DEFAULT_ZOOM_SPEED 0.25

class ComponentCamera : public Component
{
public:
	ComponentCamera(Entity* entity);
	~ComponentCamera() {};

	static inline Component::Type GetType() { return Component::Type::CAMERA; }

	const glm::mat4& GetViewProjMatrix() const { return ViewProjMatrix; }
	const glm::vec3& GetPosition() const { return position; }
	const float& GetRotation() const { return rotation; }
	const float& GetZoom() const { return zoom; }

	void SetPosition(glm::vec3 pos) { position = pos; UpdateViewMatrix(); }
	void SetRotation(float rot) { rotation = rot; UpdateViewMatrix(); }
	void SetZoom(float zoom_) { zoom = zoom_; }

	void OnResize(int width, int height);
	void OnZoom(int zoom);

	// --- CAMERA CONTROLLER
	const float& GetZoomSpeed() const { return zoom_speed; }
	const float& GetMoveSpeed() const { return move_speed; }
	void SetZoomSpeed(float speed) { zoom_speed = speed; }
	void SetMoveSpeed(float speed) { move_speed = speed; }
	void OnUpdate(float dt) override;

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float left, float right, float up, float down);

private:
	glm::vec3 position = glm::vec3(0.0f);
	float rotation = 0.0f;
	float zoom = 1.0f;

	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 ViewProjMatrix = glm::mat4(1.0f);

	// --- CAMERA CONTROLLER
	float move_speed = 0.0f;
	float zoom_speed = 0.0f;
};

