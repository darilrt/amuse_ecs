# Amuse ECS

Amuse ECS is a simple Entity Component System (ECS) library for c++. It is designed to be simple to use and easy to understand. It is also designed to be fast and efficient. 

## Example

```cpp
#include <ecs.hpp>

struct Game
{
    struct Position
    {
        float x, y;
    };

    struct Velocity
    {
        float x, y;
    };

    Game(ecs::App &app)
    {
        app.register_component<Position>();
        app.register_component<Velocity>();

        app.system<Position, Velocity>()
            .on<ecs::Update>(
                [](ecs::App &app, Position &position, Velocity &velocity)
                {
                    position.x += velocity.x;
                    position.y += velocity.y;
                });
    }
};

int main()
{
    ecs::App()
        .use<Game>()
        .run();
    return 0;
}
```
