#include "S_Debug.h"

#include "Application.h"
#include "ModuleResources.h"
#include "ModuleRenderer.h"
#include "ModuleEvent.h"
#include "ModuleScene.h"
#include "ModuleGame.h"

#include "Components.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

#include "mmgr/mmgr.h"

void S_Debug::Init()
{
	// Events
	App->event_mgr->Subscribe(this, &S_Debug::OnWorldSizeUpdate);
}

void S_Debug::Start()
{
	// Init Resources
	grid_shader = App->resources->shaders[App->resources->LoadShader("Assets/shaders.glsl", "GRID_SHADER")]->index;
	circle_texture = App->resources->LoadTexture("Assets/circle_collider.png")->index;

	// Create Grid
	CreateGrid(DEFAULT_WORLD_WIDTH, DEFAULT_WORLD_HEIGHT, DEFAULT_GRID_SPACING);
}

void S_Debug::RenderGrid()
{
	glUseProgram(grid_shader);
	glUniformMatrix4fv(glGetUniformLocation(grid_shader, "uViewProj"), 1, GL_FALSE, (GLfloat*)&App->game->GetViewProjMatrix());

	glm::vec4 color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(glGetUniformLocation(grid_shader, "uColor"), color.r, color.g, color.b, color.a);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f));
	model = glm::scale(model, glm::vec3(grid_width, grid_height, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(grid_shader, "uTransform"), 1, GL_FALSE, (GLfloat*)&model);

	glLineWidth(0.5f);
	glBindVertexArray(gridVAO);
	glDrawArrays(GL_LINES, 0, grid_vertices);
	glBindVertexArray(0);
	glLineWidth(1.0f);

	glUseProgram(0);
}

void S_Debug::RenderColliders()
{
	for (EntityIdx entity : entities)
	{
		C_Collider collider = App->scene->GetComponent<C_Collider>(entity);
		C_Transform transform = App->scene->GetComponent<C_Transform>(entity);
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
			transform.position;
			collider.center;
			App->renderer->DrawCircle(App->resources->default_shader, circle_texture, transform.position, 
				collider.radius * 2 * transform.scale.x, color);
			break;
		}

		case C_Collider::ColliderType::RECT:
		{
			App->renderer->DrawQuad(App->resources->default_shader, transform.position, transform.size * transform.scale,
				App->resources->default_tex, color, transform.rotation, transform.size * transform.scale / 2.0f);
			break;
		}

		case C_Collider::ColliderType::UNKNOWN:
			break;
		}
	}
}

void S_Debug::CreateGrid(int width, int height, int spacing)
{
	grid_width = width;
	grid_height = height;
	grid_spacing = spacing;

	int linesX = (int)height / spacing;
	int linesY = (int)width / spacing;
	grid_vertices = (linesX + linesY + 2) * 2;
	std::vector<glm::vec2> vertices(grid_vertices);

	int count = 0;
	for (int i = 0; i <= linesX; ++i) // Horizontal Lines
	{
		vertices[2 * i] = glm::vec2(0.0f, i * spacing);	// Left
		vertices[2 * i + 1] = glm::vec2(width, i * spacing);	// Right
		count += 2;
	}
	for (int i = 0; i <= linesY; ++i) // Vertical Lines
	{
		vertices[count + 2 * i] = glm::vec2(i * spacing, 0.0f);	// Top
		vertices[count + 2 * i + 1] = glm::vec2(i * spacing, height);	// Bottom
	}

	glGenVertexArrays(1, &gridVAO);
	glGenBuffers(1, &gridVBO);

	glBindVertexArray(gridVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void S_Debug::UpdateGrid(int width, int height)
{
	grid_width = width;
	grid_height = height;

	int linesX = (int)height / grid_spacing;
	int linesY = (int)width / grid_spacing;
	grid_vertices = (linesX + linesY + 2) * 2;
	std::vector<glm::vec2> vertices(grid_vertices);

	int count = 0;
	for (int i = 0; i <= linesX; ++i) // Horizontal Lines
	{
		vertices[2 * i] = glm::vec2(0.0f, i * grid_spacing);	// Left
		vertices[2 * i + 1] = glm::vec2(width, i * grid_spacing);	// Right
		count += 2;
	}
	for (int i = 0; i <= linesY; ++i) // Vertical Lines
	{
		vertices[count + 2 * i] = glm::vec2(i * grid_spacing, 0.0f);	// Top
		vertices[count + 2 * i + 1] = glm::vec2(i * grid_spacing, height);	// Bottom
	}

	glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);
}

//-------------------------------
void S_Debug::OnWorldSizeUpdate(EventWorldSizeUpdate* e)
{
	UpdateGrid(e->width, e->height);
}
