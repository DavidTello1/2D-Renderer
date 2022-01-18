#pragma once
#include "Component.h"

#include "glm/include/glm/glm.hpp"

struct Texture;
struct Shader;

class ComponentSprite : public Component
{
public:
	ComponentSprite(Entity* entity) : texture(0), shader(0), offset(0.0f), tex_size(0.0f), sprite_size(0.0f), Component(Component::Type::SPRITE, entity) {};
	~ComponentSprite() {};

	static inline Component::Type GetType() { return Component::Type::SPRITE; }

	const uint& GetShader() const { return shader; }
	const uint& GetTexture() const { return texture; }
	const glm::vec2& GetTextureSize() const { return tex_size; }
	const glm::vec2& GetSpriteSize() const { return sprite_size; }
	const glm::vec2& GetOffset() const { return offset; }

	void SetShader(uint index) { shader = index; }
	void SetTexture(uint index) { texture = index; }
	void SetTextureSize(glm::vec2 new_size) { tex_size = new_size; }
	void SetSpriteSize(glm::vec2 new_size) { sprite_size = new_size; }
	void SetOffset(glm::vec2 pos) { offset = pos; }

private:
	uint shader = 0;
	uint texture = 0;

	glm::vec2 tex_size = glm::vec2(0.0f);
	glm::vec2 sprite_size = glm::vec2(0.0f);
	glm::vec2 offset = glm::vec2(0.0f);
};


