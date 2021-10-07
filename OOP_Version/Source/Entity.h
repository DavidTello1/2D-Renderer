#pragma once
#include "Globals.h"

#include <vector>

//class Component;

class Entity
{
public:
	Entity() {};
	virtual ~Entity() {};

	UID GetUID() { return uid; }

	//Component* GetComponent(Component::Type component);
	//Component* AddComponent(Component::Type component);
	//void DeleteComponent(Component::Type component);
	//bool HasComponent(Component::Type component);

private:
	UID uid = 0;

	//std::vector<Component*> components;
};