#include "S_CameraController.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleGame.h"
#include "ModuleEvent.h"

#include "Components.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

#include "Optick/include/optick.h"
#include "mmgr/mmgr.h"

S_CameraController::S_CameraController()
{
	this->mask.set(App->scene->GetComponentType<C_Transform>());
	this->mask.set(App->scene->GetComponentType<C_Camera>());
	this->mask.set(App->scene->GetComponentType<C_CameraController>());
}

S_CameraController::~S_CameraController()
{
}

void S_CameraController::Init()
{
	App->event_mgr->Subscribe(this, &S_CameraController::OnResize);
	App->event_mgr->Subscribe(this, &S_CameraController::OnZoom);
	App->event_mgr->Subscribe(this, &S_CameraController::OnSpeedChange);
}

void S_CameraController::Update(float dt)
{
	// --- Get Components ---
	transforms.clear();
	cameras.clear();
	controllers.clear();
	for (EntityIdx entity : entities)
	{
		transforms.push_back(App->scene->GetComponent<C_Transform>(entity));
		cameras.push_back(App->scene->GetComponent<C_Camera>(entity));
		controllers.push_back(App->scene->GetComponent<C_CameraController>(entity));
	}

	for (size_t i = 0; i < entities.size(); ++i)
	{
		C_Transform& transform = transforms[i];
		C_Camera& camera = cameras[i];
		C_CameraController& controller = controllers[i];

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
		{
			transform.position = tmp_pos;

			glm::mat4 transf = glm::translate(glm::mat4(1.0f), { transform.position, 0.0f }) *
				glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation), glm::vec3(0, 0, 1));
			camera.view = glm::inverse(transf);
			camera.viewproj = camera.projection * camera.view;
		}
	}

	OPTICK_PUSH("Set Components");
	for (size_t i = 0; i < entities.size(); ++i)
	{
		App->scene->SetComponent<C_Transform>(entities[i], transforms[i]);
		App->scene->SetComponent<C_Camera>(entities[i], cameras[i]);
		App->scene->SetComponent<C_CameraController>(entities[i], controllers[i]);
	}
	OPTICK_POP();

}

void S_CameraController::OnResize(EventWindowResize* e)
{
	C_Camera& camera = App->scene->GetComponent<C_Camera>(App->game->GetMainCamera());
	C_CameraController controller = App->scene->GetComponent<C_CameraController>(App->game->GetMainCamera());

	camera.projection = glm::ortho(0.0f, e->width * controller.zoom, e->height * controller.zoom, 0.0f);
	camera.viewproj = camera.projection * camera.view;
}

void S_CameraController::OnZoom(EventCameraZoom* e)
{
	C_Camera& camera = App->scene->GetComponent<C_Camera>(App->game->GetMainCamera());
	C_CameraController& controller = App->scene->GetComponent<C_CameraController>(App->game->GetMainCamera());

	controller.zoom -= e->zoom * controller.zoom_speed;
	controller.zoom = std::max(controller.zoom, controller.zoom_speed);
	
	camera.projection = glm::ortho(0.0f, App->window->GetWidth() * controller.zoom, App->window->GetHeight() * controller.zoom, 0.0f);
	camera.viewproj = camera.projection * camera.view;
}

void S_CameraController::OnSpeedChange(EventCameraSpeedChanged* e)
{
	C_CameraController& controller = App->scene->GetComponent<C_CameraController>(App->game->GetMainCamera());
	controller.move_speed = e->speed;
}