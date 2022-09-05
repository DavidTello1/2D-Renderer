#include "S_Debug.h"

#include "Application.h"
#include "ModuleResources.h"
#include "ModuleRenderer.h"
#include "ModuleEvent.h"
#include "ModuleScene.h"
#include "ModuleGame.h"

#include "Components.h"

#include "S_Physics.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

#include "mmgr/mmgr.h"

S_Debug::S_Debug()
{
	this->mask.set(App->scene->GetComponentType<C_Collider>());
	this->mask.set(App->scene->GetComponentType<C_Transform>());
}

S_Debug::~S_Debug()
{
}

void S_Debug::Start()
{
	// Init Resources
	line_shader = App->resources->shaders[App->resources->LoadShader("Assets/shaders.glsl", "LINE_SHADER")]->index;
	circle_texture = App->resources->LoadTexture("Assets/circle_collider.png")->index;
}

void S_Debug::Update(float dt)
{
	transforms.clear();
	colliders.clear();
	for (EntityIdx entity : entities)
	{
		transforms.push_back(App->scene->GetComponent<C_Transform>(entity));
		colliders.push_back(App->scene->GetComponent<C_Collider>(entity));
	}
}

void S_Debug::RenderGrid()
{
	// Reset Buffer
	App->renderer->line_index_count = 0;
	App->renderer->line_buffer_ptr = App->renderer->line_buffer;

	// --- Grid
	const S_Physics* physics = App->scene->GetSystemPhysics();
	const FixedGrid* grid = physics->GetGrid();

	constexpr glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
	const glm::vec2& pos = grid->GetPosition();
	const glm::vec2& size = grid->GetSize();
	const float& cellSize = grid->GetCellSize();

	// Horizontal Lines
	for (size_t i = 0; i < grid->GetNumRows() + 1; ++i)
	{
		glm::vec2 start = glm::vec2(pos.x,			pos.y + cellSize * i);
		glm::vec2 end   = glm::vec2(pos.x + size.x, pos.y + cellSize * i);

		App->renderer->DrawLine(start, end, color);
	}

	// Vertical Lines
	for (size_t i = 0; i < grid->GetNumColumns() + 1; ++i)
	{
		glm::vec2 start = glm::vec2(pos.x + cellSize * i, pos.y);
		glm::vec2 end   = glm::vec2(pos.x + cellSize * i, pos.y + size.y);

		App->renderer->DrawLine(start, end, color);
	}

	// --- Actual Draw
	glUseProgram(line_shader);
	glUniformMatrix4fv(glGetUniformLocation(line_shader, "uViewProj"), 1, GL_FALSE, (GLfloat*)&App->game->GetViewProjMatrix());

	GLsizeiptr buffer_size = (uint8_t*)App->renderer->line_buffer_ptr - (uint8_t*)App->renderer->line_buffer;
	glBindBuffer(GL_ARRAY_BUFFER, App->renderer->lineVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_size, App->renderer->line_buffer);

	glLineWidth(2.0f);
	glBindVertexArray(App->renderer->lineVAO);
	glDrawArrays(GL_LINES, 0, App->renderer->line_index_count);
	glBindVertexArray(0);
	glLineWidth(1.0f);

	glUseProgram(0);
}

void S_Debug::RenderColliders()
{
	for (size_t i = 0; i < entities.size(); ++i)
	{
		C_Collider collider = colliders[i];
		C_Transform transform = transforms[i];

		if (App->game->IsInsideCamera(transform) == false) // Frustum Culling
			continue;

		glm::vec4 color;
		if (collider.is_colliding)
			color = RED;
		else
			color = BLUE;
		color.a = 0.5f;

		switch (collider.type)
		{
		case C_Collider::ColliderType::CIRCLE:
		{
			App->renderer->DrawCircle(circle_texture, transform.position, collider.radius * 2 * transform.scale.x, color);
			break;
		}

		case C_Collider::ColliderType::RECT:
		{
			App->renderer->DrawQuad(transform.position, transform.rotation, transform.size * transform.scale,
				App->resources->default_tex, color, transform.size * transform.scale / 2.0f);
			break;
		}

		case C_Collider::ColliderType::UNKNOWN:
			break;
		}
	}
}