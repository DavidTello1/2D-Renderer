#include "ModuleRenderer.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleEvent.h"
#include "ModuleGUI.h" // draw gui & debug (***?)
#include "ModuleScene.h" // draw scene & debug (***?)
#include "ModuleGame.h" //***should not be here
#include "ModuleResources.h" // for default_tex

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

	// Init Texture Slots Array
	for (size_t i = 0; i < MaxTextures; ++i)
		tex_slots[i] = 0;

	// Events
	App->event_mgr->Subscribe(this, &ModuleRenderer::OnResize);

	return ret;
}

bool ModuleRenderer::Start()
{
	// Init Texture Array in Default Shader
	tex_slots[0] = App->resources->default_tex;
	glUseProgram(App->resources->default_shader);
	int samplers[32];
	for (int i = 0; i < 32; ++i)
		samplers[i] = i;
	glUniform1iv(glGetUniformLocation(App->resources->default_shader, "uTextures"), 32, samplers);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Create Basic Quad
	CreateQuad();

	// Create Line
	CreateLine();

	return true;
}

bool ModuleRenderer::PreUpdate(float dt)
{
	// Clear Screen
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Begin Batch
	BeginBatch();

	// Reset Stats
	stats.draw_calls = 0;
	stats.quad_count = 0;

	return true;
}

bool ModuleRenderer::PostUpdate(float dt)
{
	OPTICK_CATEGORY("Renderer PostUpdate", Optick::Category::Rendering);

	GLuint shader = App->resources->default_shader;
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "uViewProj"), 1, GL_FALSE, (GLfloat*)&App->game->GetViewProjMatrix());

	// --- Render Scene
	OPTICK_PUSH("Scene Draw");
	App->scene->Draw();
	OPTICK_POP();

	// --- Colliders
	OPTICK_PUSH("Draw Colliders");
	if (App->gui->IsDrawColliders())
		App->scene->DrawColliders();
	OPTICK_POP();

	// --- End Batch and Render ---
	OPTICK_PUSH("Render Batch");
	EndBatch();
	RenderBatch();
	OPTICK_POP();

	// --- Grid
	OPTICK_PUSH("Draw Grid");
	if (App->gui->IsDrawGrid())
	{
		App->scene->DrawGrid();
		stats.draw_calls++;
	}
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
	glDeleteBuffers(1, &quadIBO);
	RELEASE_ARRAY(quad_buffer);

	glDeleteVertexArrays(1, &lineVAO);
	glDeleteBuffers(1, &lineVBO);
	RELEASE_ARRAY(line_buffer);


	return true;
}
//--------------------------------
void ModuleRenderer::BeginBatch()
{
	quad_index_count = 0;
	quad_buffer_ptr = quad_buffer;
}

void ModuleRenderer::EndBatch()
{
	// Set Quad VBO
	GLsizeiptr size = (uint8_t*)quad_buffer_ptr - (uint8_t*)quad_buffer;
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, quad_buffer);
}

void ModuleRenderer::RenderBatch()
{
	for (uint32_t i = 0; i < tex_slot_index; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, tex_slots[i]);
	}

	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, quad_index_count, GL_UNSIGNED_INT, nullptr);
	stats.draw_calls++;

	quad_index_count = 0;
	tex_slot_index = 1;
}

//--------------------------------
void ModuleRenderer::DrawQuad(const glm::mat4& transform, const glm::vec2& position, const glm::vec2& size, uint32_t texture, const glm::vec4& color)
{
	if (quad_index_count >= MaxIndexCount || tex_slot_index > 31)
	{
		EndBatch();
		RenderBatch();
		BeginBatch();
	}

	// Check if texture is already loaded
	int tex_index = 0;
	for (uint32_t i = 0; i < tex_slot_index; ++i)
	{
		if (tex_slots[i] == texture)
		{
			tex_index = int(i);
			break;
		}
	}

	// If new texture load into tex_slots array
	if (tex_index == 0)
	{
		tex_index = (int)tex_slot_index;
		tex_slots[tex_slot_index] = texture;
		tex_slot_index++;
	}

	constexpr glm::vec2 texCoords[] = { 
		{ 0.0f, 1.0f }, 
		{ 1.0f, 0.0f }, 
		{ 0.0f, 0.0f }, 
		{ 1.0f, 1.0f } 
	};

	constexpr glm::vec3 positions[] = { 
		{ 0.0f, 1.0f, 0.0f },  
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f }
	};

	// Fill buffer (4 vertices)
	for (size_t i = 0; i < 4; i++)
	{
		quad_buffer_ptr->model = transform;
		quad_buffer_ptr->position = positions[i];
		quad_buffer_ptr->color = color;
		quad_buffer_ptr->tex_coords = texCoords[i];
		quad_buffer_ptr->tex_index = tex_index;
		quad_buffer_ptr++;
	}

	quad_index_count += 6;
	stats.quad_count++;
}

void ModuleRenderer::DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const uint32_t texture, const glm::vec4& color, const glm::vec2& center)
{
	OPTICK_PUSH("Draw Quad");

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

	DrawQuad(model, position, size, texture, color);

	OPTICK_POP();
}

void ModuleRenderer::DrawCircle(const uint32_t texture, const glm::vec2& position, const float& diameter, const glm::vec4& color)
{
	OPTICK_PUSH("Draw Circle");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(diameter, diameter, 1.0f));

	DrawQuad(model, position, glm::vec2(diameter), texture, color);

	OPTICK_POP();
}

void ModuleRenderer::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
{
	OPTICK_PUSH("Draw Line");

	line_buffer_ptr->position = start;
	line_buffer_ptr->color = color;
	line_buffer_ptr++;

	line_buffer_ptr->position = end;
	line_buffer_ptr->color = color;
	line_buffer_ptr++;

	line_index_count += 2;

	OPTICK_POP();
}

//--------------------------------
void ModuleRenderer::CreateQuad()
{
	quad_buffer = new QuadVertex[MaxVertexCount];

	glCreateVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glCreateBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(QuadVertex), nullptr, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(quadVAO, 0);
	glEnableVertexArrayAttrib(quadVAO, 1);
	glEnableVertexArrayAttrib(quadVAO, 2);
	glEnableVertexArrayAttrib(quadVAO, 3);
	glEnableVertexArrayAttrib(quadVAO, 4);
	glEnableVertexArrayAttrib(quadVAO, 5);
	glEnableVertexArrayAttrib(quadVAO, 6);
	glEnableVertexArrayAttrib(quadVAO, 7);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(0 * sizeof(glm::vec4)));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(1 * sizeof(glm::vec4)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, position));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, color));
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, tex_coords));
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, tex_index));

	uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;
	for (int i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 0 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 1 + offset;

		offset += 4;
	}

	glCreateBuffers(1, &quadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void ModuleRenderer::CreateLine()
{
	line_buffer = new LineVertex[MaxVertexCount];

	glCreateVertexArrays(1, &lineVAO);
	glBindVertexArray(lineVAO);

	glCreateBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(LineVertex), nullptr, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(lineVAO, 0);
	glEnableVertexArrayAttrib(lineVAO, 1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, color));
}

void ModuleRenderer::UpdateViewportSize()
{
	glViewport(0, 0, App->window->GetWidth(), App->window->GetHeight());
}
