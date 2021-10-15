#include "Application.h"
#include "ModuleWindow.h"
#include "ComponentCamera.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

ComponentCamera::ComponentCamera(Entity* entity, glm::vec2 pos, float zoom_) : position(pos), zoom(zoom_), Component(Component::Type::CAMERA, entity)
{
	move_speed = DEFAULT_MOVE_SPEED;
	zoom_speed = DEFAULT_ZOOM_SPEED;

	UpdateMatrices();
}

void ComponentCamera::UpdateViewMatrix()
{
	ViewMatrix = glm::lookAt(glm::vec3(position, zoom), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void ComponentCamera::UpdateProjectionMatrix()
{
	ProjectionMatrix = glm::ortho(0.0f, (float)App->window->GetWidth(), (float)App->window->GetHeight(), 0.0f);
}