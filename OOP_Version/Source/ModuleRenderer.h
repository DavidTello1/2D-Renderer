#pragma once
#include "Module.h"

#include "glm/include/glm/glm.hpp"

struct Texture;
typedef void* SDL_GLContext;
typedef unsigned int GLuint;

#define MAX_TEXTURES 32;

struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 tex_coords;
	float tex_index;
};

struct Batch
{
	// Vertices
	Vertex* quad_buffer = nullptr;
	Vertex* quad_buffer_ptr = nullptr;

	// Textures
	std::vector<float> textures;
	uint32_t texture_slot_index = 1;

	// Index
	uint32_t index_count = 0;
};

// -------------------------------------------
class ModuleRenderer : public Module
{
public:
	ModuleRenderer(bool start_enabled = true);
	~ModuleRenderer();

	bool Init() override;
	bool PreUpdate(float dt) override;
	bool PostUpdate(float dt) override;
	bool CleanUp() override;

	void BeginBatch();
	void EndBatch();
	void Render();

	void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t texture);

	void DrawCircle(const glm::vec2& center, const float radius, const glm::vec4& color);

	void UpdateViewportSize();

public:
	static const size_t MaxTextures = 32;
	static const size_t MaxQuadCount = 10000;
	static const size_t MaxVertexCount = MaxQuadCount * 4;
	static const size_t MaxIndexCount = MaxQuadCount * 6;

public:
	SDL_GLContext context;

	// Quad
	GLuint quadVAO = 0;
	GLuint quadVBO = 0;
	GLuint quadIBO = 0;

	// Batch
	Batch batch;
};
