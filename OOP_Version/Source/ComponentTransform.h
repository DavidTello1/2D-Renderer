#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"
#include "glm/include/glm/gtc/type_ptr.hpp"

class ComponentTransform : public Component
{
public:
	ComponentTransform(Entity* entity, glm::vec2 pos = glm::vec2(0.0f), glm::vec2 scale_ = glm::vec2(1.0f), float rot = 0.0f) 
		: position(pos), scale(scale_), rotation(rot), Component(Component::Type::TRANSFORM, entity) 
	{
		transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) * 
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	~ComponentTransform() {};

	static inline Component::Type GetType() { return Component::Type::TRANSFORM; }

	const glm::vec2& GetPosition() const { return position; }
	void SetPosition(glm::vec2 pos) { position = pos; }

	const glm::vec2& GetScale() const { return scale; }
	void SetScale(glm::vec2 scale_) { scale = scale_; }

	const float& GetRotation() const { return rotation; }
	void SetRotation(float rot) { rotation = rot; }

	const glm::mat4& GetTransform() const { return transform; }

private:
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 scale = glm::vec2(1.0f);
	float rotation = 0.0f;

	glm::mat4 transform = glm::mat4(1.0f);
};

