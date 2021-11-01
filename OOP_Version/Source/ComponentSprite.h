#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

typedef unsigned int uint;

struct Texture;
struct Shader;

class ComponentSprite : public Component
{
public:
	ComponentSprite(Entity* entity, uint texture_index = 0, uint shader_index = 0, glm::vec2 pos = glm::vec2(0.0f), glm::vec2 size_ = glm::vec2(0.0f))
		: texture(texture_index), shader(shader_index), position(pos), size(size_), Component(Component::Type::SPRITE, entity) {};
	~ComponentSprite() {};

	static inline Component::Type GetType() { return Component::Type::SPRITE; }

	const uint& GetTexture() const { return texture; }
	const uint& GetShader() const { return shader; }
	const glm::vec2& GetSize() const { return size; }

	void SetTexture(uint index) { texture = index; }
	void SetShader(uint index) { shader = index; }
	void SetSize(glm::vec2 new_size) { size = new_size; }
	void SetPosition(glm::vec2 pos) { position = pos; }

private:
	uint texture = 0;
	uint shader = 0;

	glm::vec2 position = glm::vec2(0.0f); //position inside texture
	glm::vec2 size = glm::vec2(0.0f);
};


