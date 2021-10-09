#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

class ComponentCamera : public Component
{
public:
	ComponentCamera(Entity* entity, glm::vec2 position = glm::vec2(0.0f), float zoom = 1.0f);
	~ComponentCamera() {};

	static inline Component::Type GetType() { return Component::Type::CAMERA; }

	glm::vec2 GetPosition() const { return position; }
	void SetPosition(glm::vec2 pos) { position = pos; }

	float GetZoom() const { return zoom; }
	void SetZoom(float zoom_) { zoom = zoom_; }

	glm::mat4 GetViewProjMatrix() const { return ViewMatrix * ProjectionMatrix; }
	void UpdateMatrices() { UpdateViewMatrix(); UpdateProjectionMatrix(); }

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	glm::vec2 position;
	float zoom;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
};

