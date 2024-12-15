#pragma once

#include <string>

#include "amuse_ecs/archetype.hpp"

namespace ecs
{
    struct EntityMeta
    {
        EntityId id;
        Archetype *archetype;
        std::string name;
    };
} // namespace ecs