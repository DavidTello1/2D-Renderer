#pragma once
#include "Module.h"

#include "glm/include/glm/glm.hpp"

struct Texture;
typedef void* SDL_GLContext;
typedef unsigned int GLuint;

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

	//void Render();

	void DrawQuad(const GLuint shader, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	void DrawQuad(const GLuint shader, const glm::vec2& position, const glm::vec2& size, uint32_t texture);

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
};
