#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "amuse_ecs/world.hpp"

TEST(WorldEvent, "Event handling")
{
    ecs::World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    auto archetype_id = ecs::ArchetypeId::from_components<Position, Velocity>();

    bool called = false;

    world.on<ecs::Set>(archetype_id,
                       [&]()
                       {
                           called = true;
                       });

    world.entity()
        .add<Position>({1.0f, 2.0f})
        .add<Velocity>({3.0f, 4.0f});

    world.dispatch();

    ASSERT(called);
}