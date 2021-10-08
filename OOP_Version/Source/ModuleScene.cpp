#include "Application.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleResources.h"
#include "ModuleRenderer.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

ModuleScene::ModuleScene(bool start_enabled) : Module("ModuleScene", start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	// Set Camera
	glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 target_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	ViewMatrix = glm::lookAt(camera_pos, target_pos, glm::vec3(0.0f, 1.0f, 0.0f));
	ProjectionMatrix = glm::ortho(0.0f, (float)App->window->GetWidth(), (float)App->window->GetHeight(), 0.0f);

	//// Shader
	//App->resources->LoadShader("Assets/shaders.glsl", "DEFAULT_SHADER");

	return true;
}

bool ModuleScene::Update(float dt)
{

	return true;
}

bool ModuleScene::CleanUp()
{

	return true;
}

//--------------------------------------
void ModuleScene::Draw()
{
	// Shader
	GLuint shader = App->resources->shaders[0]->index;
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "uViewProj"), 1, GL_FALSE, (GLfloat*)&(ViewMatrix * ProjectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uTransform"), 1, GL_FALSE, (GLfloat*)&glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))); //(GLfloat*)&entity->GetComponent(transform));
	
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, App->resources->textures[0]->index);
	//glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);

	auto location = glGetUniformLocation(shader, "uTextures");
	int samplers[32];
	for (int i = 0; i < 32; ++i)
		samplers[i] = i;
	glUniform1iv(location, 32, samplers);

	// Quad
	App->renderer->DrawQuad({ 0.0f, 0.0f }, { 500.0f, 500.0f }, App->resources->textures[0]->index);
}