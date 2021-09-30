#pragma once
#include "Module.h"

typedef void* SDL_GLContext;

class ModuleRenderer : public Module
{
public:
	ModuleRenderer(bool start_enabled = true);
	~ModuleRenderer();

	bool Init();
	bool PreUpdate(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

public:
	SDL_GLContext context;
};
