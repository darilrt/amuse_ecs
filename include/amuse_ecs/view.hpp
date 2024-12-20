#pragma once

#include "amuse_ecs/archetype.hpp"

namespace ecs
{
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

        void each(std::function<void(Entity, Components &...)> &&func) const
        {
            for (auto archetype : archetypes)
            {
                for (size_t i = 0; i < archetype->entities.size(); i++)
                {
                    func(
                        archetype->find(archetype->entities[i]),
                        *static_cast<Components *>(archetype->component_data[ECS_ID(Components)][i])...);
                }
            }
        }

        void each(std::function<void(Components &...)> &&func) const
        {
            for (auto archetype : archetypes)
            {
                for (size_t i = 0; i < archetype->entities.size(); i++)
                {
                    func(*static_cast<Components *>(archetype->component_data[ECS_ID(Components)][i])...);
                }
            }
        }

    private:
        World &_world;
    };

} // namespace ecs