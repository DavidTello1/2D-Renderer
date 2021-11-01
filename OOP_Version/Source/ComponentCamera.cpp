#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ComponentCamera.h"

#include <algorithm>
#include "glm/include/glm/gtc/type_ptr.hpp"

ComponentCamera::ComponentCamera(Entity* entity, glm::vec3 pos, float zoom_) : position(pos), zoom(zoom_), ViewMatrix(1.0f), Component(Component::Type::CAMERA, entity)
{
	move_speed = DEFAULT_MOVE_SPEED;
	zoom_speed = DEFAULT_ZOOM_SPEED;

	OnResize(App->window->GetWidth(), App->window->GetHeight());
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
	ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	ViewProjMatrix = ProjectionMatrix * ViewMatrix;
}

void ComponentCamera::OnUpdate(float dt)
{
	glm::vec3 tmp_pos = position;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_DOWN || 
		App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT) // Up
	{
		tmp_pos.y += move_speed * dt;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_DOWN || 
		App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT) // Down
	{
		tmp_pos.y -= move_speed * dt;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_DOWN || 
		App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT) // Left
	{
		tmp_pos.x -= move_speed * dt;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_DOWN || 
		App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT) // Right
	{
		tmp_pos.x += move_speed * dt;
	}

	if (tmp_pos != position)
		SetPosition(tmp_pos);
}

void ComponentCamera::OnResize(float width, float height)
{
	UpdateProjectionMatrix(0, width * zoom, 0, height * zoom);
}

void ComponentCamera::OnZoom(float new_zoom)
{
	float width = App->window->GetWidth();
	float height = App->window->GetHeight();

	zoom -= new_zoom * zoom_speed;
	zoom = std::max(zoom, zoom_speed);
	UpdateProjectionMatrix(0, width * zoom, 0, height * zoom);
	UpdateViewMatrix();
}