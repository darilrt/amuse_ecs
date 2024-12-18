#pragma once

#include "amuse_ecs/world.hpp"
#include "amuse_ecs/event_handler.hpp"

namespace ecs
{
    EVENT(Start);
    EVENT(PreUpdate);
    EVENT(Update);
    EVENT(PostUpdate);

    class App;

    template <typename... Components>
    class SystemBuilder
    {
        App &_app;
        World &_world;
        EventHandler &_event_handler;

    public:
        SystemBuilder(App &app, World &world, EventHandler &event_handler)
            : _app(app), _world(world), _event_handler(event_handler)
        {
        }

        template <typename Event>
        SystemBuilder<Components...> &on([[maybe_unused]] const std::function<void(App &, Components &...)> &&func)
        {
            if constexpr (std::is_same_v<Event, Set>)
            {
                _world.on<Set>(
                    _world.get_archetype_id<Components...>(),
                    [func = std::move(func), *this]()
                    {
                        _world.view<Components...>().each(
                            [func, *this](Components &...components)
                            {
                                func(_app, components...);
                            });
                    });
            }
            else
            {
                _event_handler.on<Event>(
                    [func = std::move(func), *this]()
                    {
                        _world.view<Components...>().each(
                            [func, *this](Components &...components)
                            {
                                func(_app, components...);
                            });
                    });
            }

            return *this;
        }
    };

    class App
    {
        World _world;
        EventHandler _event_handler;

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

        inline EventHandler &event_handler()
        {
            return _event_handler;
        }

        void run();

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