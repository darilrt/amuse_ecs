#pragma once

#include "amuse_ecs/archetype.hpp"

class World;

template <typename... Components>
class View
{
public:
    std::vector<Archetype *> archetypes;

    View(World &world) : _world(world)
    {
    }

    inline World &world() { return _world; }

    size_t size() const
    {
        size_t size = 0;
        for (auto archetype : archetypes)
        {
            size += archetype->entities.size();
        }
        return size;
    }

    typedef std::function<void(Entity, Components &...)> Func;

    void each(Func &&func) const
    {
        for (auto archetype : archetypes)
        {
            for (size_t i = 0; i < archetype->entities.size(); i++)
            {
                func(
                    archetype->find(archetype->entities[i]),
                    *static_cast<Components *>(archetype->component_data[ECS_ID(Components)].at(i))...);
            }
        }
    }

private:
    World &_world;
};