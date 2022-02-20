#pragma once

#include <set>

typedef unsigned int EntityIdx;

struct System
{
    std::set<EntityIdx> entities;
};
