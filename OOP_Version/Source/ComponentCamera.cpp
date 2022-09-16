#include "ComponentCamera.h"

#include "Application.h"
#include "ModuleWindow.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

ComponentCamera::ComponentCamera(Entity* entity) : position(0.0f), rotation(0.0f), Component(Component::Type::CAMERA, entity)
{
	UpdateProjectionMatrix(0, (float)App->window->GetWidth(), (float)App->window->GetHeight(), 0);
	UpdateViewMatrix();
}

void ComponentCamera::UpdateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

	ViewMatrix = glm::inverse(transform);
	ViewProjMatrix = ProjectionMatrix * ViewMatrix;
}

void ComponentCamera::UpdateProjectionMatrix(float left, float right, float bottom, float top)
{
	ProjectionMatrix = glm::ortho(left, right, bottom, top);
	ViewProjMatrix = ProjectionMatrix * ViewMatrix;
}