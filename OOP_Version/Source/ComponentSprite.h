#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

typedef unsigned int uint;

struct Texture;
struct Shader;

class ComponentSprite : public Component
{
public:
	ComponentSprite(Entity* entity) : texture(0), shader(0), offset(0.0f), size(0.0f), Component(Component::Type::SPRITE, entity) {};
	~ComponentSprite() {};

	static inline Component::Type GetType() { return Component::Type::SPRITE; }

	const uint& GetTexture() const { return texture; }
	const uint& GetShader() const { return shader; }
	const glm::vec2& GetSize() const { return size; }
	const glm::vec2& GetOffset() const { return offset; }

	void SetTexture(uint index) { texture = index; }
	void SetShader(uint index) { shader = index; }

	void SetSize(glm::vec2 new_size) { size = new_size; }
	void SetOffset(glm::vec2 pos) { offset = pos; }

private:
	uint texture = 0;
	uint shader = 0;

	glm::vec2 size = glm::vec2(0.0f);
	glm::vec2 offset = glm::vec2(0.0f);
};


