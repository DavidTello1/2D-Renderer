#pragma once
#include "Module.h"

#define TITLE "OOP Version - David Tello"
#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:
	ModuleWindow(bool start_enabled = true);
	virtual ~ModuleWindow();

	bool Init();
	bool Start();
	bool CleanUp();

	SDL_Window* GetWindow() const { return window; }
	uint GetWidth() const { return screen_width; }
	uint GetHeight() const { return screen_height; }

	void SetWidth(uint width, bool update_size = true);
	void SetHeigth(uint height, bool update_size = true);

	uint GetRefreshRate() const;
	void GetMaxMinSize(uint& min_width, uint& min_height, uint& max_width, uint& max_height) const;

private:
	SDL_Window* window = nullptr; //The window we'll be rendering to
	SDL_Surface* screen_surface = nullptr; //The surface contained by the window

	uint screen_width = 1024;
	uint screen_height = 768;
};
