#pragma once
#include <set>
typedef unsigned int Entity;

struct SystemBase
{
    std::set<Entity> mEntities;
};
