#pragma once
#include "Component.h"

class ComponentAnimation : public Component
{
public:
	ComponentAnimation(Entity* entity) : Component(Component::Type::ANIMATION, entity) {};
	~ComponentAnimation() {};

	static inline Component::Type GetType() { return Component::Type::ANIMATION; }

};

