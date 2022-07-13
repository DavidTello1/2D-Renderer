#include "ModuleRenderer.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleEvent.h"
#include "ModuleGUI.h" // draw gui & debug (***?)
#include "ModuleScene.h" // draw scene & debug (***?)
#include "ModuleGame.h" //***should not be here

#include "Imgui/imgui.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "glm/include/glm/gtc/type_ptr.hpp"

#pragma comment(lib, "glew/x64/glew32.lib")
#pragma comment (lib, "glu32.lib")    // link OpenGL Utility lib
#pragma comment (lib, "opengl32.lib") // link Microsoft OpenGL lib

#include "Optick/include/optick.h"
#include "mmgr/mmgr.h"

ModuleRenderer::ModuleRenderer(bool start_enabled) : Module("ModuleRenderer", start_enabled)
{
	context = NULL;
}

ModuleRenderer::~ModuleRenderer()
{
}

bool ModuleRenderer::Init()
{
	LOG("Creating 3D Renderer context\n");
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
	LOG("Loading glew");

	// OpenGL
	glViewport(0, 0, App->window->GetWidth(), App->window->GetHeight());
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create Basic Quad
	CreateQuad();

	App->event_mgr->Subscribe(this, &ModuleRenderer::OnResize);

	return ret;
}

bool ModuleRenderer::PreUpdate(float dt)
{
	// Clear Screen & Set Viewport
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset Stats
	stats.draw_calls = 0;
	stats.quad_count = 0;

	return true;
}

bool ModuleRenderer::PostUpdate(float dt)
{
	OPTICK_CATEGORY("Renderer PostUpdate", Optick::Category::Rendering);

	// --- Render Scene
	OPTICK_PUSH("Scene Draw");
	App->scene->Draw();
	OPTICK_POP();

	// --- Debug Draw
	OPTICK_PUSH("Draw Colliders");
	App->scene->DrawDebug(App->gui->IsDrawGrid(), App->gui->IsDrawColliders());
	OPTICK_POP();

	OPTICK_PUSH("Draw Grid");
	if (App->gui->IsDrawGrid())
		stats.draw_calls++;
	OPTICK_POP();

	// --- Render ImGui
	OPTICK_PUSH("Draw Imgui");
	App->gui->Draw();
	OPTICK_POP();

	SDL_GL_SwapWindow(App->window->GetWindow());
	return true;
}

bool ModuleRenderer::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);

	return true;
}

//--------------------------------
void ModuleRenderer::DrawQuad(const uint shader, const glm::vec2& position, const glm::vec2& size, const uint32_t texture, 
	const glm::vec4& color, const float& rotation, const glm::vec2& center)
{
	OPTICK_PUSH("Draw Quad");

	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "uViewProj"), 1, GL_FALSE, (GLfloat*)&App->game->GetViewProjMatrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	if (center != glm::vec2(0.0f))
	{
		model = glm::translate(model, glm::vec3(center, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-center, 0.0f));
	}
	else
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uTransform"), 1, GL_FALSE, (GLfloat*)&model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);

	glUniform4f(glGetUniformLocation(shader, "uColor"), color.r, color.g, color.b, color.a);

	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	stats.draw_calls++;
	stats.quad_count++;

	OPTICK_POP();
}

void ModuleRenderer::DrawCircle(const uint shader, const uint32_t texture, const glm::vec2& position, const float& diameter, const glm::vec4& color)
{
	OPTICK_PUSH("Draw Circle");

	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "uViewProj"), 1, GL_FALSE, (GLfloat*)&App->game->GetViewProjMatrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(diameter, diameter, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uTransform"), 1, GL_FALSE, (GLfloat*)&model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);

	glUniform4f(glGetUniformLocation(shader, "uColor"), color.r, color.g, color.b, color.a);

	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	stats.draw_calls++;
	stats.quad_count++;

	OPTICK_POP();
}

//--------------------------------
void ModuleRenderer::CreateQuad()
{
	float vertices[] = {
		// positions        // texture coords
		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,	1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		0, 3, 1  // second triangle
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadIBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void ModuleRenderer::UpdateViewportSize()
{
	glViewport(0, 0, App->window->GetWidth(), App->window->GetHeight());
}
