#include "S_CameraController.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleGame.h"
#include "ModuleEvent.h"

#include "Components.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

#include "mmgr/mmgr.h"

void S_CameraController::Init()
{
	App->event_mgr->Subscribe(this, &S_CameraController::OnResize);
	App->event_mgr->Subscribe(this, &S_CameraController::OnZoom);
	App->event_mgr->Subscribe(this, &S_CameraController::OnSpeedChange);
}

void S_CameraController::Update(float dt)
{
	for (EntityIdx entity : entities)
	{
		C_Transform& transform = App->scene->GetComponent<C_Transform>(entity);
		C_CameraController& controller = App->scene->GetComponent<C_CameraController>(entity);

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

void S_CameraController::OnResize(EventWindowResize* e)
{
	C_Camera& camera = App->scene->GetComponent<C_Camera>(App->game->GetMainCamera());
	C_CameraController controller = App->scene->GetComponent<C_CameraController>(App->game->GetMainCamera());

	camera.projection = glm::ortho(0.0f, e->width * controller.zoom, e->height * controller.zoom, 0.0f);
}

void S_CameraController::OnZoom(EventCameraZoom* e)
{
	C_Camera& camera = App->scene->GetComponent<C_Camera>(App->game->GetMainCamera());
	C_CameraController& controller = App->scene->GetComponent<C_CameraController>(App->game->GetMainCamera());

	controller.zoom -= e->zoom * controller.zoom_speed;
	controller.zoom = std::max(controller.zoom, controller.zoom_speed);
	
	camera.projection = glm::ortho(0.0f, App->window->GetWidth() * controller.zoom, App->window->GetHeight() * controller.zoom, 0.0f);
}

void S_CameraController::OnSpeedChange(EventCameraSpeedChanged* e)
{
	C_CameraController& controller = App->scene->GetComponent<C_CameraController>(App->game->GetMainCamera());
	controller.move_speed = e->speed;
}