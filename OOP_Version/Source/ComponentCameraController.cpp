#include "ComponentCameraController.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

#include "Entity.h"
#include "ComponentCamera.h"

#include <algorithm>
#include "glm/include/glm/glm.hpp"

ComponentCameraController::ComponentCameraController(Entity* entity) 
	: move_speed(DEFAULT_MOVE_SPEED), zoom_speed(DEFAULT_ZOOM_SPEED), Component(Component::Type::CAMERA_CONTROLLER, entity) 
{
	// Initialize Camera, if there is no ComponentCamera create one
	camera = (ComponentCamera*)entity->GetComponent(Component::Type::CAMERA);
	if (camera == nullptr)
		camera = (ComponentCamera*)entity->AddComponent(Component::Type::CAMERA);
};

void ComponentCameraController::OnUpdate(float dt)
{
	glm::vec3 tmp_pos = camera->GetPosition();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_DOWN ||
		App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT) // Up
	{
		tmp_pos.y -= move_speed * dt;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_DOWN ||
		App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT) // Down
	{
		tmp_pos.y += move_speed * dt;
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

	if (tmp_pos != camera->GetPosition())
		camera->SetPosition(tmp_pos);
}

void ComponentCameraController::OnResize(int width, int height)
{
	camera->UpdateProjectionMatrix(0, width * zoom, height * zoom, 0);
}

void ComponentCameraController::OnZoom(int new_zoom)
{
	zoom -= new_zoom * zoom_speed;
	zoom = std::max(zoom, zoom_speed);
	OnResize(App->window->GetWidth(), App->window->GetHeight());
}