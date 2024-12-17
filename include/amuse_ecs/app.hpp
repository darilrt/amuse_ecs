#pragma once

#include "amuse_ecs/world.hpp"

namespace ecs
{
    EVENT(Start);
    EVENT(PreUpdate);
    EVENT(Update);
    EVENT(PostUpdate);

    class App
    {
        World _world;

    public:
        template <typename Module>
        App &use()
        {
            Module(*this);
            return *this;
        }

        inline World &world()
        {
            return _world;
        }

        void run();

        template <typename Component>
        inline void register_component()
        {
            _world.register_component<Component>();
        }

        template <typename... Components>
        void system()
        {
            // TODO: Implement system
        }
    };
} // namespace ecs