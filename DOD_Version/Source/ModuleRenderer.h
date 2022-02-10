#pragma once
#include "Module.h"

#include "glm/include/glm/glm.hpp"

class Grid;
struct Texture;
struct EventWindowResize;
typedef void* SDL_GLContext;

struct RenderStats
{
	uint quad_count = 0;
	uint draw_calls = 0;
};

// -------------------------------------------
class ModuleRenderer : public Module
{
public:
	ModuleRenderer(bool start_enabled = true);
	virtual ~ModuleRenderer();

	bool Init() override;
	bool PreUpdate(float dt) override;
	bool PostUpdate(float dt) override;
	bool CleanUp() override;

	void UpdateViewportSize();

	const SDL_GLContext& GetContext() const { return context; }
	const RenderStats& GetStats() const { return stats; }

	void DrawQuad(const uint shader, const glm::vec2& position, const glm::vec2& size, uint32_t texture, 
		const glm::vec4& color = glm::vec4(1.0f), const float& rotation = 0.0f, const glm::vec2& center = glm::vec2(0.0f)); //*** CLEAN

	void DrawCircle(const uint shader, const glm::vec2& center, const float& radius, const glm::vec4& color);

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

	// Grid
	Grid* grid = nullptr;
};
