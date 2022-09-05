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

struct QuadVertex {
	glm::mat4 model;
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 tex_coords;
	int tex_index;
};

struct LineVertex {
	glm::vec2 position;
	glm::vec4 color;
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

	void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color = glm::vec4(1.0f));

	// --- EVENTS ---
	void OnResize(EventWindowResize* e) { UpdateViewportSize(); }

private:
	void CreateQuad();
	void CreateLine();

private:
	SDL_GLContext context;
	RenderStats stats;

	// Quad
	uint quadVAO = 0;
	uint quadVBO = 0;
	uint quadIBO = 0;

	uint quad_index_count = 0;
	QuadVertex* quad_buffer = nullptr;
	QuadVertex* quad_buffer_ptr = nullptr;

	// Textures
	std::array<uint32_t, MaxTextures> tex_slots{};
	uint32_t tex_slot_index = 1;

public: // public for debug drawing (not best solution)
	// Line
	uint lineVAO = 0;
	uint lineVBO = 0;

	uint line_index_count = 0;
	LineVertex* line_buffer = nullptr;
	LineVertex* line_buffer_ptr = nullptr;
};
