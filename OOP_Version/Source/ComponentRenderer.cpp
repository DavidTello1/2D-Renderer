#include "ComponentRenderer.h"

#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleScene.h"

#include "Entity.h"
#include "ComponentSprite.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "Glew/include/glew.h"

void ComponentRenderer::Draw(GLuint shader)
{
	Entity* entity_ = GetEntity();

	ComponentSprite* sprite = (ComponentSprite*)entity_->GetComponent(Component::Type::SPRITE);
	if (sprite == nullptr)
		return;

	ComponentTransform* transform = (ComponentTransform*)entity_->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	glUniformMatrix4fv(glGetUniformLocation(shader, "uViewProj"), 1, GL_FALSE, (GLfloat*)&App->scene->main_camera->GetViewProjMatrix());
	glUniformMatrix4fv(glGetUniformLocation(shader, "uTransform"), 1, GL_FALSE, (GLfloat*)&transform->GetTransform());
	auto location = glGetUniformLocation(shader, "uTextures");
	int samplers[32];
	for (int i = 0; i < 32; ++i)
		samplers[i] = i;
	glUniform1iv(location, 32, samplers);

	// Quad
	App->renderer->DrawQuad(transform->GetPosition(), sprite->GetSize() * transform->GetScale(), sprite->GetTexture());
}