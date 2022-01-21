#pragma once
#include "EntityManager.h"

#include <unordered_map>

class ComponentArrayBase 
{
public:
	virtual ~ComponentArrayBase() = default;
};

template<typename T>
class ComponentArray : public ComponentArrayBase
{
public:
	// Add Component
	template<typename T>
	int AddComponent(Entity entity, T& component)
	{
		if (entity_to_index.find(entity) != entity_to_index.end())
		{
			LOG("--- Unable to Add Component: entity already has component of same type");
			return -1;
		}

		int index = component_count; // get index of component to add (first empty spot -> component_count)
		components[index] = component; // add component data to component_array at index

		entity_to_index[entity] = index; // add index to entity_to_index map
		index_to_entity[index] = entity; // add entity to index_to_entity_map

		++component_count; // set new index for next component to be added

		return index;
	}

	// Remove Component
	template<typename T>
	void RemoveComponent(Entity entity)
	{
		if (entity_to_index.find(entity) == entity_to_index.end())
		{
			LOG("--- Unable to Remove Component: component not found");
			return;
		}

		size_t index = entity_to_index[entity]; // get index of entity to remove
		size_t last_index = component_count - 1; // get index of last component
		components[index] = components[last_index]; // copy last component data to removed component index

		Entity moved_entity = index_to_entity[last_index]; // get entity of last component (the copied one)
		entity_to_index[moved_entity] = index; // update entity_to_index map
		index_to_entity[index] = moved_entity; // update index_to_entity map

		entity_to_index.erase(entity); // delete removed entity from entity_to_index map
		index_to_entity.erase(last_index); // delete last index from index_to_entity map
		--component_count;
	}

	// Get Component
	template<typename T>
	T* GetComponent(Entity entity)
	{
		if (entity_to_index.find(entity) == entity_to_index.end())
		{
			LOG("--- Component not found");
			return;
		}

		return components[entity_to_index[entity]];
	}

private:
	std::array<T, MAX_ENTITIES> components; // array of component_data - is always tightly packed
	size_t component_count;

	std::unordered_map<Entity, size_t> entity_to_index; // map to keep where each entity component_data's index is
	std::unordered_map<size_t, Entity> index_to_entity; // just a copy of entity_to_index but reversed - used for removing
};
