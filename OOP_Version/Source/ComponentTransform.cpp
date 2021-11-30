#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

void ComponentTransform::UpdateTransform()
{
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(Normalize(position), 0.0f)) *
		glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
}

glm::vec2 ComponentTransform::Normalize(glm::vec2 vec)
{
	return glm::vec2(vec.x / App->window->GetWidth(), vec.y / App->window->GetHeight());
}
