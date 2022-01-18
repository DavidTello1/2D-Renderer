#pragma once
#include "Entity.h"

struct ComponentManager
{
	ComponentManager(size_t elementsize) { // We'll allocate enough memory to hold MAX_ENTITIES, each with element size
		elementSize = elementsize;
		pData = new char[elementSize * MAX_ENTITIES];
	}

	~ComponentManager() {
		delete[] pData;
	}

	inline void* get(size_t index) // looking up the component at the desired index
	{
		return pData + index * elementSize;
	}

	char* pData{ nullptr };
	size_t elementSize{ 0 };
};
