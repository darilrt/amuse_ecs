#pragma once

#include <string>

#include "amuse_ecs/archetype.hpp"

namespace ecs
{
    struct EntityMeta
    {
        EntityId id = 0;
        Archetype *archetype = nullptr;
        std::string name = "";
    };
} // namespace ecs