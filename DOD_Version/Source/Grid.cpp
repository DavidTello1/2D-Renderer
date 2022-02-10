#include "Grid.h"

#include "Application.h"
#include "ModuleEvent.h"
#include "ModuleResources.h"
#include "ModuleGame.h" //*** for GetViewProjMatrix in Grid::Draw()

#include "Glew/include/glew.h"
#include "glm/include/glm/gtc/type_ptr.hpp"
#include <vector>

Grid::Grid()
{
	App->event_mgr->Subscribe(this, &Grid::OnWorldSizeUpdate);
	grid_shader = App->resources->shaders[App->resources->LoadShader("Assets/shaders.glsl", "GRID_SHADER")]->index;
}

void Grid::Create(int width, int height, int spacing)
{
	grid_width = width;
	grid_height = height;

	int linesX = (int)width / spacing;
	int linesY = (int)height / spacing;
	int num_lines = (linesX + linesY + 1) * 4;

	std::vector<glm::vec2> vertices(num_lines);
	for (int i = 0; i <= 10; ++i)
	{
		vertices[4 * i + 0] = glm::vec2(0.0f, i * spacing);	// X top
		vertices[4 * i + 1] = glm::vec2(width, i * spacing);	// X bottom

		vertices[4 * i + 2] = glm::vec2(i * spacing, 0.0f);	// Y top
		vertices[4 * i + 3] = glm::vec2(i * spacing, height);	// Y bottom
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

void Grid::Update(int width, int height)
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

void Grid::Draw()
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

//--------------------------------
void Grid::OnWorldSizeUpdate(EventWorldSizeUpdate* e)
{
	Update(e->width, e->height);
}
