#pragma once
#include <set>
typedef unsigned int EntityIdx;

struct SystemBase
{
    std::set<EntityIdx> entities;
};
