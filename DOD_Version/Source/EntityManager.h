#pragma once
#include "Globals.h"

#include <queue>
#include <array>
#include <bitset>

const int MAX_ENTITIES = 10000;
const int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

struct Entity {
	UID id;
	ComponentMask mask;
};

class EntityManager
{
public:
	EntityManager()
	{
		for (size_t index = 0; index < MAX_ENTITIES; ++index)
			available_indexes.push(index);
	}

	Entity CreateEntity()
	{
		if (entity_count > MAX_ENTITIES)
		{
			LOG("--- Unable to create entity: max entities reached");
			return { 0, ComponentMask() };
		}

		size_t index = available_indexes.front();
		available_indexes.pop();
		++entity_count;

		UID id = 1; //CreateUID();
		Entity entity = { id, ComponentMask() };
		entities[index] = entity;

		return entity;
	}

	void DeleteEntity(Entity entity) 
	{
		int index = -1;
		//for (int i = 0; i < entities.size() - 1; ++i)
		//{
		//	if (entities[i] == entity) 
		//	{
		//		index = i;
		//		break;
		//	}
		//}

		if (index == -1)
		{
			LOG("--- Unable to delete entity: index not found");
			return;
		}

		entities[index].id = 0;
		entities[index].mask.reset();

		available_indexes.push(index);
		--entity_count;
	}

private:
	std::array<Entity, MAX_ENTITIES> entities;

	std::queue<size_t> available_indexes;
	uint32_t entity_count = 0;
};