#include "ComponentRenderer.h"

#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleScene.h"

#include "Entity.h"
#include "ComponentSprite.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentCircleCollider.h"
#include "ComponentRectCollider.h"

#include "Glew/include/glew.h"

void ComponentRenderer::Draw(GLuint shader)
{
	Entity* entity = GetEntity();

	ComponentSprite* sprite = (ComponentSprite*)entity->GetComponent(Component::Type::SPRITE);
	if (sprite == nullptr)
		return;

	ComponentTransform* transform = (ComponentTransform*)entity->GetComponent(Component::Type::TRANSFORM);
	if (transform == nullptr)
		return;

	glUseProgram(shader);

	//glUniformMatrix4fv(glGetUniformLocation(shader, "uViewProj"), 1, GL_FALSE, (GLfloat*)&App->scene->main_camera->GetViewProjMatrix());
	glUniformMatrix4fv(glGetUniformLocation(shader, "uTransform"), 1, GL_FALSE, (GLfloat*)&transform->GetTransform());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->GetTexture());
	glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);

	glBindVertexArray(App->renderer->quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
