#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "amuse_ecs/entity.hpp"
#include "amuse_ecs/world.hpp"

TEST(ComponentRegistration, "Component registration")
{
    World world;
    world.register_component<Position>();

    ASSERT(world.component_deleters.size() == 1);
}

TEST(ComponentAddition, "Component addition")
{
    World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    [[maybe_unused]] Entity entity0 = world.entity()
                                          .add<Position>({1.0f, 2.0f})
                                          .add<Velocity>({3.0f, 4.0f});

    Entity entity1 = world.entity();

    world.dispatch();

    entity1.add<Position>({1.0f, 2.0f});
    entity1.add<Velocity>({3.0f, 4.0f});

    world.dispatch();

    Archetype &archetype = world.get_archetype<Position, Velocity>();

    ASSERT(archetype.entities.size() == 2);

    auto pos = static_cast<Position *>(archetype.get_component(entity0.id(), ECS_ID(Position)));
    auto vel = static_cast<Velocity *>(archetype.get_component(entity0.id(), ECS_ID(Velocity)));

    ASSERT(pos != nullptr);
    ASSERT(vel != nullptr);

    ASSERT(pos->x == 1.0f);
    ASSERT(pos->y == 2.0f);

    ASSERT(vel->x == 3.0f);
    ASSERT(vel->y == 4.0f);

    pos = static_cast<Position *>(archetype.get_component(entity1.id(), ECS_ID(Position)));
    vel = static_cast<Velocity *>(archetype.get_component(entity1.id(), ECS_ID(Velocity)));

    ASSERT(pos != nullptr);
    ASSERT(vel != nullptr);

    ASSERT(pos->x == 1.0f);
    ASSERT(pos->y == 2.0f);

    ASSERT(vel->x == 3.0f);
    ASSERT(vel->y == 4.0f);
}

TEST(ComponentGet, "Component get")
{
    World world;
    world.register_component<Position>();

    Entity entity = world.entity().add<Position>({1.0f, 2.0f});

    world.dispatch();

    ASSERT(entity.has<Position>());

    auto pos = entity.get<Position>();

    ASSERT(pos != nullptr);

    ASSERT(pos->x == 1.0f);
    ASSERT(pos->y == 2.0f);
}

TEST(ComponentRemoval, "Component removal")
{
    World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    Entity entity = world.entity()
                        .add<Position>({1.0f, 2.0f})
                        .add<Velocity>({3.0f, 4.0f});

    world.dispatch();

    entity.remove<Position>();

    world.dispatch();

    std::cout << entity.meta().archetype << std::endl;

    ASSERT(!entity.has<Position>());
    ASSERT(entity.has<Velocity>());

    entity.remove<Velocity>();

    world.dispatch();

    ASSERT(!entity.has<Velocity>());
}
