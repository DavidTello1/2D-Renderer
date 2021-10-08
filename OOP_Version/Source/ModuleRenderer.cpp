#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"
#include "ModuleResources.h"
#include "ModuleScene.h"

#include "Imgui/imgui.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "glm/include/glm/gtx/transform.hpp"

#pragma comment(lib, "glew/libx86/glew32.lib")
#pragma comment (lib, "glu32.lib")    // link OpenGL Utility lib
#pragma comment (lib, "opengl32.lib") // link Microsoft OpenGL lib

ModuleRenderer::ModuleRenderer(bool start_enabled) : Module("ModuleRenderer", start_enabled)
{
	context = NULL;
}

ModuleRenderer::~ModuleRenderer()
{}

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

	// --- Batch ---
	batch.quad_buffer = new Vertex[MaxVertexCount];

	glCreateVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glCreateBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexArrayAttrib(quadVAO, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
	glEnableVertexArrayAttrib(quadVAO, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));
	glEnableVertexArrayAttrib(quadVAO, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, tex_coords));
	glEnableVertexArrayAttrib(quadVAO, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, tex_index));

	uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;
	for (int i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	glCreateBuffers(1, &quadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Default Texture (white 1x1)
	glCreateTextures(GL_TEXTURE_2D, 1, &default_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	uint32_t color = 0xffffffff;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

	// Init Textures
	batch.textures.reserve(MaxTextures);
	batch.textures.push_back(default_tex_slot);
	batch.textures.push_back(App->resources->LoadTexture("Assets/asteroids.png")->index);
	for (size_t i = 2; i < MaxTextures; i++)
		batch.textures.emplace_back(0.0f);

	return ret;
}

bool ModuleRenderer::PreUpdate(float dt)
{
	// Clear Screen & Set Viewport
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}

bool ModuleRenderer::PostUpdate(float dt)
{
	// Render Scene
	BeginBatch();
	App->scene->Draw();
	EndBatch();
	Render();

	// Render ImGui
	App->gui->Draw();

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

	glDeleteTextures(1, &default_tex);

	delete[] batch.quad_buffer;

	return true;
}

//--------------------------------
void ModuleRenderer::BeginBatch()
{
	batch.quad_buffer_ptr = batch.quad_buffer;
}

void ModuleRenderer::EndBatch()
{
	GLsizeiptr size = (uint8_t*)batch.quad_buffer_ptr - (uint8_t*)batch.quad_buffer;
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, batch.quad_buffer);
}

void ModuleRenderer::Render()
{
	for (uint32_t i = 0; i < batch.texture_slot_index; i++)
		glBindTextureUnit(i, batch.textures[i]);

	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, batch.index_count, GL_UNSIGNED_INT, nullptr);

	batch.index_count = 0;
	batch.texture_slot_index = 1;
}

void ModuleRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (batch.index_count >= MaxIndexCount)
	{
		EndBatch();
		Render();
		BeginBatch();
	}

	float texture_index = 0.0f;

	batch.quad_buffer_ptr->position = { position.x, position.y, 0.0f };
	batch.quad_buffer_ptr->color = color;
	batch.quad_buffer_ptr->tex_coords = { 0.0f, 0.0f };
	batch.quad_buffer_ptr->tex_index = texture_index;
	batch.quad_buffer_ptr++;

	batch.quad_buffer_ptr->position = { position.x + size.x, position.y, 0.0f };
	batch.quad_buffer_ptr->color = color;
	batch.quad_buffer_ptr->tex_coords = { 1.0f, 0.0f };
	batch.quad_buffer_ptr->tex_index = texture_index;
	batch.quad_buffer_ptr++;

	batch.quad_buffer_ptr->position = { position.x + size.x, position.y + size.y, 0.0f };
	batch.quad_buffer_ptr->color = color;
	batch.quad_buffer_ptr->tex_coords = { 1.0f, 1.0f };
	batch.quad_buffer_ptr->tex_index = texture_index;
	batch.quad_buffer_ptr++;

	batch.quad_buffer_ptr->position = { position.x, position.y + size.y, 0.0f };
	batch.quad_buffer_ptr->color = color;
	batch.quad_buffer_ptr->tex_coords = { 0.0f, 1.0f };
	batch.quad_buffer_ptr->tex_index = texture_index;
	batch.quad_buffer_ptr++;

	batch.index_count += 6;
}

void ModuleRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t texture)
{
	if (batch.index_count >= MaxIndexCount || batch.texture_slot_index > MaxTextures - 1)
	{
		EndBatch();
		Render();
		BeginBatch();
	}

	constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	float texture_index = 0.0f;
	for (uint32_t i = 1; i < batch.texture_slot_index; ++i)
	{
		if (batch.textures[i] == texture)
		{
			texture_index = (float)i;
			break;
		}
	}

	if (texture_index == 0.0f)
	{
		texture_index = (float)batch.texture_slot_index;
		batch.textures[batch.texture_slot_index] = texture;
		batch.texture_slot_index++;
	}

	batch.quad_buffer_ptr->position = { position.x, position.y, 0.0f };
	batch.quad_buffer_ptr->color = color;
	batch.quad_buffer_ptr->tex_coords = { 0.0f, 0.0f };
	batch.quad_buffer_ptr->tex_index = texture_index;
	batch.quad_buffer_ptr++;

	batch.quad_buffer_ptr->position = { position.x + size.x, position.y, 0.0f };
	batch.quad_buffer_ptr->color = color;
	batch.quad_buffer_ptr->tex_coords = { 1.0f, 0.0f };
	batch.quad_buffer_ptr->tex_index = texture_index;
	batch.quad_buffer_ptr++;

	batch.quad_buffer_ptr->position = { position.x + size.x, position.y + size.y, 0.0f };
	batch.quad_buffer_ptr->color = color;
	batch.quad_buffer_ptr->tex_coords = { 1.0f, 1.0f };
	batch.quad_buffer_ptr->tex_index = texture_index;
	batch.quad_buffer_ptr++;

	batch.quad_buffer_ptr->position = { position.x, position.y + size.y, 0.0f };
	batch.quad_buffer_ptr->color = color;
	batch.quad_buffer_ptr->tex_coords = { 0.0f, 1.0f };
	batch.quad_buffer_ptr->tex_index = texture_index;
	batch.quad_buffer_ptr++;

	batch.index_count += 6;
}

void ModuleRenderer::UpdateViewportSize()
{
	glViewport(0, 0, App->window->GetWidth(), App->window->GetHeight());
}
