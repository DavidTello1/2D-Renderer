#include "ComponentTransform.h"

#include "glm/include/glm/gtc/type_ptr.hpp"

void ComponentTransform::UpdateTransform()
{
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
		glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
}
