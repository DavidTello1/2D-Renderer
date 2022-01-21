#pragma once
#include "ComponentArray.h"

#include <memory>

class ComponentManager
{
public:
	// Add Component
	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->AddComponent(entity, component);
	}

	// Remove Component
	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveComponent(entity);
	}

	// Get Component
	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetComponent(entity);
	}

	// Register Component
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		if (component_types.find(typeName) != component_types.end())
		{
			LOG("---Unable to register component: component is already registered");
			return;
		}

		component_types.insert({ typeName, type_count });
		component_arrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		++type_count;
	}

	// Get Component Type (used for creating component masks)
	template<typename T>
	std::uint8_t GetComponentType()
	{
		const char* typeName = typeid(T).name();

		if (component_types.find(typeName) == component_types.end())
		{
			LOG("Unable to get component type: type does not exist");
			return;
		}

		return component_types[typeName];
	}

private:
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		if (component_types.find(typeName) == component_types.end())
		{
			LOG("--- Component Type does not exist");
			return nullptr;
		}

		return std::static_pointer_cast<ComponentArray<T>>(component_arrays[typeName]);
	}

private:
	std::unordered_map<const char*, std::shared_ptr<ComponentArrayBase>> component_arrays{}; // map of component_arrays by name

	std::unordered_map<const char*, std::uint8_t> component_types{}; // map of component_type number by name
	std::uint8_t type_count;
};
