#pragma once

#include "amuse_ecs/world.hpp"
#include "amuse_ecs/event_handler.hpp"

namespace ecs
{
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

        // This function registers the given function to be called when the specified event is emitted
        template <typename Event>
        SystemBuilder<Components...> &on([[maybe_unused]] const std::function<void(App &, Components &...)> &&func)
        {
            if constexpr (std::is_same_v<Event, Set> || std::is_same_v<Event, Unset>)
            {
                _world.on<Event>(
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

        // This function executes the given function for each entity that has the specified components
        // in the moment of the call
        SystemBuilder<Components...> each(const std::function<void(App &, Components &...)> &func)
        {
            _world.view<Components...>().each(
                [func, *this](Components &...components)
                {
                    func(_app, components...);
                });

            return *this;
        }
    };
} // namespace esc
