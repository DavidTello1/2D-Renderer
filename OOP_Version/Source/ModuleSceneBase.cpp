#include "ModuleSceneBase.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "ModuleGUI.h"

#include "ComponentCameraController.h"

ModuleSceneBase::ModuleSceneBase(bool start_enabled) : Module("ModuleScene", start_enabled)
{
	pcg32_srandom_r(&rng, 42u, 54u); // Seed the RNG with a fixed constant
}

ModuleSceneBase::~ModuleSceneBase()
{
}

void ModuleSceneBase::OnWindowResize()
{
	App->gui->is_update_pos = true;

	App->renderer->UpdateViewportSize();

	if (main_camera != nullptr) 
		main_camera->OnResize(App->window->GetWidth(), App->window->GetHeight());

}

void ModuleSceneBase::OnMouseZoom(int mouse_wheel)
{
	if (main_camera != nullptr) 
		main_camera->OnZoom(mouse_wheel);
}

const glm::mat4& ModuleSceneBase::GetViewProjMatrix() const
{
	return main_camera->GetCamera()->GetViewProjMatrix();
}