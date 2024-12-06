#pragma once

#include <vector>
#include <functional>
#include <algorithm>

#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/entity.hpp"
#include "amuse_ecs/world.hpp"

template <typename... Components>
class View
{
public:
    World &world;
    size_t index = -1;
    std::vector<Archetype *> archetypes;

    View(World &world) : world(world)
    {
        for (auto &archetype : world.archetypes)
        {
            if (archetype->id.contains(world.generate_archetype_id<Components...>()))
            {
                archetypes.push_back(archetype.get());
            }
        }
    }

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

    void each(std::function<void(Entity, Components *...)> func)
    {
        while (next())
        {
            auto &archetype = archetypes[index];

            for (size_t i = 0; i < archetype->entities.size(); i++)
            {
                Entity entity = world.get_entity(archetype->entities[i]);
                std::tuple<Components *...> components =
                    std::make_tuple(entity.get<Components>()...);

                func(entity, std::get<Components *>(components)...);
            }
        }
    }
};