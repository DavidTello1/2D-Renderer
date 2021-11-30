#include "Application.h"
#include "ModuleWindow.h"

#include "mmgr/mmgr.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module("ModuleWindow", start_enabled)
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		SDL_DisplayMode DM;
		SDL_GetDesktopDisplayMode(0, &DM);

		//Create window
		screen_width = DEFAULT_WIDTH;
		screen_height = DEFAULT_HEIGHT;

		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

		//Use OpenGL 3.2
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, flags);

		if (window == nullptr)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError(), 'd');
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	return ret;
}

bool ModuleWindow::Start()
{
	// Force to trigger a chain of events to refresh aspect ratios	
	SDL_SetWindowSize(window, screen_width, screen_height);

	return true;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != nullptr)
		SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetWidth(uint width, bool update_size)
{
	if (update_size) 
		SDL_SetWindowSize(window, width, GetHeight());
	screen_width = width;
}

void ModuleWindow::SetHeigth(uint height, bool update_size)
{
	if (update_size)
		SDL_SetWindowSize(window, GetWidth(), height);
	screen_height = height;
}

uint ModuleWindow::GetRefreshRate() const
{
	uint ret = 0;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	else
		ret = dm.refresh_rate;

	return ret;
}

void ModuleWindow::GetMaxMinSize(uint& min_width, uint& min_height, uint& max_width, uint& max_height) const
{
	min_width = 480;
	min_height = 320;
	max_width = 3000;
	max_height = 2000;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	else
	{
		max_width = dm.w;
		max_height = dm.h;
	}
}
