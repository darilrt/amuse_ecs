#include "amuse_ecs/app.hpp"

namespace ecs
{

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
