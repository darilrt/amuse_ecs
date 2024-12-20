#include "amuse_ecs/app.hpp"

namespace ecs
{
    App::App() : _entity(_world, 0, nullptr)
    {
        _entity = _world.entity("ecs::App::entity");
        _world.dispatch();
        _entity = _world.find(_entity.id());
    }

    void App::run()
    {
        _event_handler.emit<Start>();

        _running = true;

        while (_running)
        {
            _event_handler.emit<FrameStart>();
            _world.dispatch();

            _event_handler.emit<PreUpdate>();
            _world.dispatch();

            _event_handler.emit<Update>();
            _world.dispatch();

            _event_handler.emit<PostUpdate>();
            _world.dispatch();

            _event_handler.emit<FrameEnd>();
            _world.dispatch();
        }
    }

} // namespace ecs
