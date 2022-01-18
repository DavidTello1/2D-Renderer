#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

class ComponentCamera : public Component
{
public:
	ComponentCamera(Entity* entity);
	~ComponentCamera() {};

	static inline Component::Type GetType() { return Component::Type::CAMERA; }

	const glm::vec3& GetPosition() const { return position; }
	const float& GetRotation() const { return rotation; }

	void SetPosition(glm::vec3 pos) { position = pos; UpdateViewMatrix(); }
	void SetRotation(float rot) { rotation = rot; UpdateViewMatrix(); }

	void UpdateProjectionMatrix(float left, float right, float up, float down);
	const glm::mat4& GetViewProjMatrix() const { return ViewProjMatrix; }

private:
	void UpdateViewMatrix();

private:
	glm::vec3 position = glm::vec3(0.0f);
	float rotation = 0.0f;

	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 ViewProjMatrix = glm::mat4(1.0f);
};

