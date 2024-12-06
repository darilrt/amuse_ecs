#pragma once

#include <vector>
#include <functional>
#include <algorithm>

#include "amuse_ecs/archetype.hpp"

template <typename... Components>
class View
{
public:
    size_t index = -1;
    std::vector<Archetype *> archetypes;

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

    void each(std::function<void(EntityId, Components *...)> func)
    {
        while (next())
        {
            auto &archetype = archetypes[index];
            auto &component_data = archetype->component_data;

            for (size_t i = 0; i < archetype->entities.size(); i++)
            {
                std::tuple<Components *...> components =
                    std::make_tuple(
                        (Components *)component_data[archetype->component_index[std::type_index(typeid(Components))]].data() + i...);

                func(archetype->entities[i], std::get<Components *>(components)...);
            }
        }
    }
};