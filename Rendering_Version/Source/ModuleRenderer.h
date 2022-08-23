#pragma once
#include "Module.h"
#include <array>

#include "glm/include/glm/glm.hpp"

struct Texture;
struct EventWindowResize;
typedef void* SDL_GLContext;

struct RenderStats {
	uint quad_count = 0;
	uint draw_calls = 0;
};

struct Vertex {
	glm::mat4 model;
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 tex_coords;
	int tex_index;
};

static const size_t MaxQuadCount = 10000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;
static const size_t MaxTextures = 32;

// -------------------------------------------
class ModuleRenderer : public Module
{
public:
	ModuleRenderer(bool start_enabled = true);
	virtual ~ModuleRenderer();

	bool Init() override;
	bool Start() override;
	bool PreUpdate(float dt) override;
	bool PostUpdate(float dt) override;
	bool CleanUp() override;

	void UpdateViewportSize();

	const SDL_GLContext& GetContext() const { return context; }
	const RenderStats& GetStats() const { return stats; }

	// --- BATCHES ---
	void BeginBatch();
	void EndBatch();
	void RenderBatch();

	// --- DRAWING ---
	void DrawQuad(const glm::mat4& transform, const glm::vec2& position, const glm::vec2& size, uint32_t texture, const glm::vec4& color = glm::vec4(1.0f));
	void DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const uint32_t texture,
		const glm::vec4& color = glm::vec4(1.0f), const glm::vec2& center = glm::vec2(0.0f));

	void DrawCircle(const uint32_t texture, const glm::vec2& center, const float& radius, const glm::vec4& color);

	// --- EVENTS ---
	void OnResize(EventWindowResize* e) { UpdateViewportSize(); }

private:
	void CreateQuad();

private:
	SDL_GLContext context;
	RenderStats stats;

	// Quad
	uint quadVAO = 0;
	uint quadVBO = 0;
	uint quadIBO = 0;

	uint index_count = 0;
	Vertex* quad_buffer = nullptr;
	Vertex* quad_buffer_ptr = nullptr;

	// Textures
	std::array<uint32_t, MaxTextures> tex_slots;
	uint32_t tex_slot_index = 1;
};
