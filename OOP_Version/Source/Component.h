#pragma once
#include "Globals.h"

class Entity;

class Component
{
public:
	enum class Type
	{
		TRANSFORM,
		CAMERA,
		CAMERA_CONTROLLER,
		RENDERER,
		SPRITE,
		RECT_COLLIDER,
		CIRCLE_COLLIDER,
		ASTEROID,
		UNKNOWN
	};

public:
	Component(Component::Type type, Entity* entity) : type(type), entity(entity) {}
	virtual ~Component() {};

	Component::Type GetType() const { return type; }
	Entity* GetEntity() const { return entity; }

	virtual void OnUpdate(float dt) {};

private:
	mutable Component::Type type;
	Entity* entity = nullptr;
};

