#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "amuse_ecs/world.hpp"

TEST(WorldEvent, "Event handling")
{
    ecs::World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    bool called_position = false;
    bool called_velocity = false;
    bool called_position_velocity = false;

    world.on<ecs::Set>(
        ecs::ArchetypeId::from_components<Position>(),
        [&]()
        {
            called_position = true;
        });

    world.on<ecs::Set>(
        ecs::ArchetypeId::from_components<Velocity>(),
        [&]()
        {
            called_velocity = true;
        });

    world.on<ecs::Set>(
        ecs::ArchetypeId::from_components<Position, Velocity>(),
        [&]()
        {
            called_position_velocity = true;
        });

    world.entity()
        .add<Position>({1.0f, 2.0f})
        .add<Velocity>({3.0f, 4.0f});

    world.dispatch();

    ASSERT(called_position);
    ASSERT(called_velocity);
    ASSERT(called_position_velocity);
}