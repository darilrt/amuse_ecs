#Amuse ECS

Amuse ECS is a simple Entity Component System (ECS) library for c++. It is designed to be simple to use and easy to understand. It is also designed to be fast and efficient. 

## Example

```cpp
#include <ecs.hpp>

struct Position {
    float x, y;
};

struct Velocity {
    float x, y;
};

int main() {
    ecs::World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    world.entity()
        .add<Position>({ 0.0f, 0.0f })
        .add<Velocity>({ 1.0f, 1.0f });

    while (true) {
        world.for_each<Position, Velocity>([](ecs::Entity entity, Position& position, Velocity& velocity) {
            position.x += velocity.x;
            position.y += velocity.y;
        });
    }

    return 0;
}
```