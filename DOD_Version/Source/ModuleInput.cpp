#include "ModuleInput.h"

#include "Application.h"
#include "ModuleEvent.h" 

#include "ImGui/imgui.h"
#include "imGui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Optick/include/optick.h"
#include "mmgr/mmgr.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module("ModuleInput", start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
	close = false;
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");

	SDL_Init(0);
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError(), 'e');
		return false;
	}

	return true;
}

// Called every draw update
bool ModuleInput::PreUpdate(float dt)
{
	OPTICK_CATEGORY("Input PreUpdate", Optick::Category::Input);

	SDL_PumpEvents();
	mouse_motion_x = mouse_motion_y = 0;

	// Check keyboard input
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	// Check mouse input
	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
	for (int i = 0; i < 5; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	// Mouse events
	SDL_Event e;
	static const char* dir;
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch (e.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[e.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[e.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouse_motion_x = e.motion.xrel;
			mouse_motion_y = e.motion.yrel;
			mouse_x = e.motion.x;
			mouse_y = e.motion.y;
			break;

		case SDL_MOUSEWHEEL:
			mouse_wheel = e.wheel.y;
			App->event_mgr->Publish(new EventCameraZoom(mouse_wheel));
			break;

		case SDL_WINDOWEVENT:
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				App->event_mgr->Publish(new EventWindowResize(e.window.data1, e.window.data2));
				break;
			}
			break;

		case SDL_QUIT:
			close = true;
			break;
		}
	}
	return true;
}

bool ModuleInput::PostUpdate(float dt)
{
	return !close; // if close == true return false and stop execution
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
