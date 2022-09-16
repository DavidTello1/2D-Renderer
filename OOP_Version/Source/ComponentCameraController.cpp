#include "ComponentCameraController.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"

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
	float world_width = (float)App->scene->GetWorldWidth();
	float world_height = (float)App->scene->GetWorldHeight();

	glm::vec3 tmp_pos = camera->GetPosition();

	if (App->window->GetWidth() * zoom > world_width) // Center X Axis
		tmp_pos.x = (world_width - App->window->GetWidth() * zoom) / 2;
	else
	{
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

		// Camera Limits (X Axis)
		if (tmp_pos.x < 0) tmp_pos.x = 0;
		else if (tmp_pos.x > world_width - App->window->GetWidth() * zoom) tmp_pos.x = world_width - App->window->GetWidth() * zoom;
	}

	if (App->window->GetHeight() * zoom > world_height) // Center Y Axis
		tmp_pos.y = (world_height - App->window->GetHeight() * zoom) / 2;
	else
	{
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

		// Camera Limits (Y Axis)
		if (tmp_pos.y < 0) tmp_pos.y = 0;
		else if (tmp_pos.y > world_height - App->window->GetHeight() * zoom) tmp_pos.y = world_height - App->window->GetHeight() * zoom;
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
