#include "Application.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleResources.h"
#include "ModuleScene.h"
#include "ModuleGame.h"
#include "ModulePhysics.h"
#include "ModuleRenderer.h"

#include <windows.h>

#include "mmgr/mmgr.h"

Application::Application()
{
	dt = 0.0f;
	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 0;
	fps_counter = 0;

	modules.push_back(window = new ModuleWindow());
	modules.push_back(input = new ModuleInput());
	modules.push_back(gui = new ModuleGUI());
	modules.push_back(resources = new ModuleResources());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(game = new ModuleGame());
	modules.push_back(physics = new ModulePhysics());
	modules.push_back(renderer = new ModuleRenderer());
}

// ---------------------------------------------
Application::~Application()
{
	for (Module* mod : modules)
		RELEASE(mod);
}

// ---------------------------------------------
bool Application::Init()
{
	bool ret = true;

	// We init everything, even if not enabled
	for (uint i = 0; i < modules.size() && ret; ++i)
	{
		if (modules[i]->IsEnabled() == true)
			ret = modules[i]->Init();
	}

	// Another round, just before starting the Updates. Only called for "active" modules
	// we send the configuration again in case a module needs it
	for (uint i = 0; i < modules.size() && ret; ++i)
	{
		if (modules[i]->IsEnabled() == true)
			ret = modules[i]->Start();
	}

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
bool Application::Update()
{
	bool ret = true;
	PrepareUpdate();

	for (uint i = 0; i < modules.size() && ret; ++i)
		if (modules[i]->IsEnabled() == true)
			ret = modules[i]->PreUpdate(dt);

	for (uint i = 0; i < modules.size() && ret; ++i)
		if (modules[i]->IsEnabled() == true)
			ret = modules[i]->Update(dt);

	for (uint i = 0; i < modules.size() && ret; ++i)
		if (modules[i]->IsEnabled() == true)
			ret = modules[i]->PostUpdate(dt);

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	// Recap on framecount and fps
	++frames;
	++fps_counter;

	if (fps_timer.Read() >= 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	// cap fps
	if (capped_ms > 0 && (last_frame_ms < capped_ms))
		SDL_Delay(capped_ms - last_frame_ms);

	// log fps
	gui->LogFPS((float)last_fps, (float)last_frame_ms);
}

// ---------------------------------------------
bool Application::CleanUp()
{
	bool ret = true;

	for (uint i = 0; i < modules.size(); i++)
		if (modules[i]->IsEnabled() == true)
			ret = modules[i]->CleanUp();

	return ret;
}

// ---------------------------------------------
uint Application::GetFramerateLimit() const
{
	if (capped_ms > 0)
		return (uint)((1.0f / (float)capped_ms) * 1000.0f);
	else
		return 0;
}

// ---------------------------------------------
void Application::SetFramerateLimit(uint max_framerate)
{
	if (max_framerate > 0)
		capped_ms = 1000 / max_framerate;
	else
		capped_ms = 0;
}
