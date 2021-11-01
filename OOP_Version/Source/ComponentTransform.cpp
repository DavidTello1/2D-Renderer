#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

ComponentTransform::ComponentTransform(Entity* entity, glm::vec2 pos, glm::vec2 scale_, float rot) : position(pos), scale(scale_), rotation(rot), Component(Component::Type::TRANSFORM, entity)
{
	UpdateTransform();
}

void ComponentTransform::UpdateTransform()
{
	glm::vec2 pos = glm::vec2(position.x / App->window->GetWidth(), position.y / App->window->GetHeight());

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}