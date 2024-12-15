#pragma once

#include "amuse_ecs/world.hpp"

namespace ecs
{
    class App
    {
    public:
        World world;

        App();
        ~App();

        template <typename Module>
        App &use()
        {
            Module(*this);
            return *this;
        }

        void run();

        template <typename Component>
        inline void register_component()
        {
            world.register_component<Component>();
        }

        template <typename... Components>
        void system()
        {
            // TODO: Implement system
        }
    };
} // namespace ecs