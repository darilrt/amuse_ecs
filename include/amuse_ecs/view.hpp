#pragma once

#include <vector>
#include <functional>
#include <algorithm>

#include "amuse_ecs/archetype.hpp"

class Entity;
class World;

Entity ecs_get_entity(EntityId id, World &world);

template <typename... Components>
class View
{
public:
    World &world;
    size_t index = -1;
    std::vector<Archetype *> archetypes;

    View(World &world) : world(world) {}

    bool next()
    {
        index++;

        if (index >= archetypes.size())
        {
            return false;
        }

        return true;
    }

    Archetype &get()
    {
        return *archetypes[index];
    }

    void
    each(std::function<void(Entity, Components *...)> func)
    {
        while (next())
        {
            auto &archetype = archetypes[index];
            auto &component_data = archetype->component_data;

            for (size_t i = 0; i < archetype->entities.size(); i++)
            {
                Entity entity = ecs_get_entity(archetype->entities[i], world);
                // std::tuple<Components *...> components =
                //     std::make_tuple(entity.get<Components>()...);

                func(entity, std::get<Components *>(components)...);
            }
        }
    }
};