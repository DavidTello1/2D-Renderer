#pragma once

class Entity;

class Component
{
public:
	enum class Type
	{
		TRANSFORM,
		CAMERA,
		RENDERER,
		SPRITE,
		COLLIDER,
		ANIMATION,
		ASTEROID,
		UNKNOWN
	};

public:
	Component(Component::Type type, Entity* entity) : type(type), entity(entity) {}
	virtual ~Component() {};

	Component::Type GetType() const { return type; }
	Entity* GetEntity() const { return entity; }

private:
	mutable Component::Type type;
	Entity* entity = nullptr;
};

