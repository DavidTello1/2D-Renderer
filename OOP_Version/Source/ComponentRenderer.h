#pragma once
#include "Component.h"

struct Shader;

typedef unsigned int GLuint;

class ComponentRenderer : public Component
{
public:
	ComponentRenderer(Entity* entity) : Component(Component::Type::RENDERER, entity) {};
	~ComponentRenderer() {};

	static inline Component::Type GetType() { return Component::Type::RENDERER; }

	void Draw();
};

