#include "Systems.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleGame.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"
#include "ModuleRenderer.h"

#include "Components.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

void S_CameraController::Init()
{
}

void S_CameraController::Update(float dt)
{
	for (Entity entity : mEntities)
	{
		C_Transform& transform = App->scene->coordinator.GetComponent<C_Transform>(entity);
		C_CameraController& controller = App->scene->coordinator.GetComponent<C_CameraController>(entity);

		float world_width = (float)App->game->GetWorldWidth();
		float world_height = (float)App->game->GetWorldHeight();

		glm::vec2 tmp_pos = transform.position;

		if (App->window->GetWidth() * controller.zoom > world_width) // Center X Axis
			tmp_pos.x = (world_width - App->window->GetWidth() * controller.zoom) / 2;
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_DOWN ||
				App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT) // Left
			{
				tmp_pos.x -= controller.move_speed * dt;
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_DOWN ||
				App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT) // Right
			{
				tmp_pos.x += controller.move_speed * dt;
			}

			// Camera Limits (X Axis)
			if (tmp_pos.x < 0) 
				tmp_pos.x = 0;
			else if (tmp_pos.x > world_width - App->window->GetWidth() * controller.zoom) 
				tmp_pos.x = world_width - App->window->GetWidth() * controller.zoom;
		}

		if (App->window->GetHeight() * controller.zoom > world_height) // Center Y Axis
			tmp_pos.y = (world_height - App->window->GetHeight() * controller.zoom) / 2;
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_DOWN ||
				App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT) // Up
			{
				tmp_pos.y -= controller.move_speed * dt;
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_DOWN ||
				App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT) // Down
			{
				tmp_pos.y += controller.move_speed * dt;
			}

			// Camera Limits (Y Axis)
			if (tmp_pos.y < 0) 
				tmp_pos.y = 0;
			else if (tmp_pos.y > world_height - App->window->GetHeight() * controller.zoom) 
				tmp_pos.y = world_height - App->window->GetHeight() * controller.zoom;
		}

		if (tmp_pos != transform.position)
			transform.position = tmp_pos;
	}
}

void S_CameraController::OnResize(int width, int height)
{
	App->gui->is_update_pos = true;
	App->renderer->UpdateViewportSize();

	C_Camera& camera = App->scene->coordinator.GetComponent<C_Camera>(App->game->GetMainCamera());
	C_CameraController controller = App->scene->coordinator.GetComponent<C_CameraController>(App->game->GetMainCamera());

	camera.projection = glm::ortho(0.0f, width * controller.zoom, height * controller.zoom, 0.0f);
}

void S_CameraController::OnZoom(int new_zoom)
{
	C_CameraController& controller = App->scene->coordinator.GetComponent<C_CameraController>(App->game->GetMainCamera());

	controller.zoom -= new_zoom * controller.zoom_speed;
	controller.zoom = std::max(controller.zoom, controller.zoom_speed);

	OnResize(App->window->GetWidth(), App->window->GetHeight());
}
