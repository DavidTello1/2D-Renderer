#include "ModuleDebug.h"

#include "Application.h"
#include "ModuleResources.h"
#include "ModuleEvent.h"
#include "ModuleRenderer.h" // for DrawColliders()
#include "ModuleGame.h" // for viewproj & defines DEFAULT_WORLD_SIZE

#include "glm/include/glm/gtc/type_ptr.hpp"

#include "mmgr/mmgr.h"

ModuleDebug::ModuleDebug(bool start_enabled) : Module("ModuleDebug", start_enabled), is_draw_grid(false), is_draw_colliders(false)
{
}

ModuleDebug::~ModuleDebug()
{
}

bool ModuleDebug::Init()
{
	App->event_mgr->Subscribe(this, &ModuleDebug::OnWorldSizeUpdate);

	return true;
}

bool ModuleDebug::Start()
{
	grid_shader = App->resources->shaders[App->resources->LoadShader("Assets/shaders.glsl", "GRID_SHADER")]->index;
	circle_texture = App->resources->LoadTexture("Assets/circle_collider.png")->index;

	CreateGrid(DEFAULT_WORLD_WIDTH, DEFAULT_WORLD_HEIGHT, DEFAULT_GRID_SPACING);

	return true;
}

bool ModuleDebug::Update(float dt)
{
	return true;
}

bool ModuleDebug::CleanUp()
{
	return true;
}

//--------------------------------
void ModuleDebug::DrawColliders()
{
	//// Rect Colliders
	//for (ComponentRectCollider* rect : rect_colliders)
	//{
	//	ComponentTransform* transform = (ComponentTransform*)rect->GetEntity()->GetComponent(Component::Type::TRANSFORM);
	//	if (transform == nullptr)
	//		continue;

	//	glm::vec4 color;
	//	if (rect->IsColliding())
	//		color = RED;
	//	else
	//		color = BLUE;
	//	color.a = 0.5f;

	//	App->renderer->DrawQuad(App->resources->default_shader, rect->GetPosition(), rect->GetSize() * transform->GetScale(),
	//		App->resources->default_tex, color);
	//}

	//// Circle Colliders
	//for (ComponentCircleCollider* circle : circle_colliders)
	//{
	//	ComponentTransform* transform = (ComponentTransform*)circle->GetEntity()->GetComponent(Component::Type::TRANSFORM);
	//	if (transform == nullptr)
	//		continue;

	//	glm::vec4 color;
	//	if (circle->IsColliding())
	//		color = RED;
	//	else
	//		color = BLUE;
	//	color.a = 0.5f;

	//	App->renderer->DrawCircle(App->resources->default_shader, circle_texture, circle->GetPosition(), circle->GetRadius() * 2 * transform->GetScale().x, color);
	//}
}

void ModuleDebug::DrawGrid()
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

void ModuleDebug::CreateGrid(int width, int height, int spacing)
{
	grid_width = width;
	grid_height = height;
	grid_spacing = spacing;

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

void ModuleDebug::UpdateGrid(int width, int height)
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

//--------------------------------
void ModuleDebug::OnWorldSizeUpdate(EventWorldSizeUpdate* e)
{
	UpdateGrid(e->width, e->height);
}
