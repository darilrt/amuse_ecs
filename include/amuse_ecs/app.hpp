#pragma once

#include "amuse_ecs/world.hpp"
#include "amuse_ecs/event_handler.hpp"
#include "amuse_ecs/system_builder.hpp"

namespace ecs
{
    // Start app when App::run is called
    EVENT(Start);

    // Frame start event
    EVENT(FrameStart);

    // Before updating the world on main loop
    EVENT(PreUpdate);

    // Update the world on main loop
    EVENT(Update);

    // After updating the world on main loop
    EVENT(PostUpdate);

    // Last event in the main loop
    EVENT(FrameEnd);

    class App
    {
        World _world;
        EventHandler _event_handler;
        bool _running = false;
        Entity _entity;

    public:
        App();

        template <typename Component>
        inline Entity add(const Component &component)
        {
            return _entity.add(component);
        }

        template <typename Component>
        inline Component *get() const
        {
            return _entity.get<Component>();
        }

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

        inline EventHandler &event_handler()
        {
            return _event_handler;
        }

        void run();

        inline void stop()
        {
            _running = false;
        }

        template <typename Component>
        inline void register_component()
        {
            _world.register_component<Component>();
        }

        template <typename... Components>
        SystemBuilder<Components...> system()
        {
            return SystemBuilder<Components...>(*this, _world, _event_handler);
        }
    };
} // namespace ecs