#include <iostream>

#define ECS_IMPLEMENTATION
#include "ecs.hpp"

struct Position
{
    float x;
    float y;
};

int main()
{
    World world;
    world.register_component<Position>();

    world.create_entity()
        .add<Position>({1.0f, 2.0f});

    world.system<Position>([](Entity entity, Position &position)
                           { std::cout << "Entity " << entity.get_id() << " has position (" << position.x << ", " << position.y << ")" << std::endl; });

    return 0;
}