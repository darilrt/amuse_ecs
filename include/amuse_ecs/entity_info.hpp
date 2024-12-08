#pragma once

#include <string>

#include "amuse_ecs/archetype.hpp"

struct EntityMeta
{
    EntityId id;
    Archetype *archetype;
    std::string name;
};
