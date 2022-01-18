#pragma once
#include <bitset>
#include <vector>

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
	EntityManager() {};
	virtual ~EntityManager();

	void Draw();

	// Create Entity
	UID CreateEntity() {
		entities.push_back({ entities.size(), ComponentMask() });
		return entities.back().id;
	}

	// Delete Entity
	void DeleteEntity(Entity* entity);

	// Add Component
	template<typename T>
	void AddComponent(UID id) {
		int componentId = GetId<T>();
		entities[id].mask.set(componentId, true);
	}
	
	// Remove Component
	template<typename T>
	void RemoveComponent(UID id) {
		int componentId = GetId<T>();
		entities[id].mask.set(componentId, false);
	}

	Component* GetComponent(Component::Type component) const;

private:
	std::vector<Entity> entities;

};
