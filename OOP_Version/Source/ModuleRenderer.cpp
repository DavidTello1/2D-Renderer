#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"

#include "Imgui/imgui.h"
#include "SDL/include/SDL.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

#pragma comment(lib, "glew/libx86/glew32.lib")
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer::ModuleRenderer(bool start_enabled) : Module("ModuleRenderer", start_enabled)
{
	context = NULL;
}

// Destructor
ModuleRenderer::~ModuleRenderer()
{}

// Called before render is available
bool ModuleRenderer::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->GetWindow());
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	// Initialize glew
	GLenum error = glewInit();
	LOG("Loading glew", 'd');

	glEnable(GL_TEXTURE_2D);

	return ret;
}

// PreUpdate: clear buffer
bool ModuleRenderer::PreUpdate(float dt)
{
	return true;
}

// PostUpdate present buffer to screen
bool ModuleRenderer::PostUpdate(float dt)
{
	App->gui->Draw();

	SDL_GL_SwapWindow(App->window->GetWindow());

	return true;
}

// Called before quitting
bool ModuleRenderer::CleanUp()
{
	LOG("Destroying 3D Renderer");
	SDL_GL_DeleteContext(context);

	return true;
}
