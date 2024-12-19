#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "amuse_ecs/entity.hpp"
#include "amuse_ecs/world.hpp"
#include "amuse_ecs/archetype_id.hpp"

TEST(ComponentRegistration, "Component registration")
{
    ecs::World world;
    world.register_component<Position>();

    ASSERT(world.component_deleters.size() == 1);
}

TEST(ComponentAddition, "Component addition")
{
    ecs::World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    ecs::Entity entity0 = world.entity()
                              .add<Position>({1.0f, 2.0f})
                              .add<Velocity>({3.0f, 4.0f});

    ecs::Entity entity1 = world.entity();

    world.dispatch();

    entity1.add<Position>({1.0f, 2.0f});
    entity1.add<Velocity>({3.0f, 4.0f});

    world.dispatch();

    ecs::Archetype &archetype = world.get_archetype<Position, Velocity>();

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
    ecs::World world;
    world.register_component<Position>();

    ecs::Entity entity = world.entity().add<Position>({1.0f, 2.0f});

    world.dispatch();

    ASSERT(entity.has<Position>());

    auto pos = entity.get<Position>();

    ASSERT(pos != nullptr);

    ASSERT(pos->x == 1.0f);
    ASSERT(pos->y == 2.0f);
}

TEST(ComponentRemoval, "Component removal")
{
    ecs::World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    ecs::Entity entity = world.entity()
                             .add<Position>({1.0f, 2.0f})
                             .add<Velocity>({3.0f, 4.0f});

    world.dispatch();

    entity.remove<Position>();

    world.dispatch();

    ASSERT(!entity.has<Position>());
    ASSERT(entity.has<Velocity>());

    entity.remove<Velocity>();

    world.dispatch();

    ASSERT(!entity.has<Velocity>());
}

TEST(ComponentEventSet, "Testing on set event for components")
{
    ecs::World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    int set_p_called = 0;
    int set_v_called = 0;
    int set_pv_called = 0;

    world.on<ecs::Set>(
        ecs::ArchetypeId::from_components<Position>(),
        [&set_p_called]()
        {
            set_p_called++;
        });

    world.on<ecs::Set>(
        ecs::ArchetypeId::from_components<Velocity>(),
        [&set_v_called]()
        {
            set_v_called++;
        });

    world.on<ecs::Set>(
        ecs::ArchetypeId::from_components<Position, Velocity>(),
        [&set_pv_called]()
        {
            set_pv_called++;
        });

    world.entity()
        .add<Position>({1.0f, 2.0f})
        .add<Velocity>({3.0f, 4.0f});

    auto ent = world.entity();
    world.dispatch();

    ASSERT(set_p_called == 1);
    ASSERT(set_v_called == 1);
    ASSERT(set_pv_called == 1);

    ent.add<Position>({1.0f, 2.0f})
        .add<Velocity>({3.0f, 4.0f});

    world.dispatch();

    ASSERT(set_p_called == 2);
    ASSERT(set_v_called == 2);
    ASSERT(set_pv_called == 2);
}

TEST(ComponentEventUnset, "Testing on unset event for components")
{
    ecs::World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    bool unset_p_called = false;
    bool unset_v_called = false;
    bool unset_pv_called = false;

    world.on<ecs::Unset>(
        ecs::ArchetypeId::from_components<Position>(),
        [&unset_p_called]()
        {
            unset_p_called = true;
        });

    world.on<ecs::Unset>(
        ecs::ArchetypeId::from_components<Velocity>(),
        [&unset_v_called]()
        {
            unset_v_called = true;
        });

    world.on<ecs::Unset>(
        ecs::ArchetypeId::from_components<Position, Velocity>(),
        [&unset_pv_called]()
        {
            unset_pv_called = true;
        });

    ecs::Entity entity = world.entity()
                             .add<Position>({1.0f, 2.0f})
                             .add<Velocity>({3.0f, 4.0f});

    world.dispatch();

    entity.remove<Position>();

    world.dispatch();

    ASSERT(unset_p_called);
    ASSERT(unset_pv_called);

    entity.remove<Velocity>();

    world.dispatch();

    ASSERT(unset_v_called);

    auto entity1 = world.entity()
                       .add<Position>({1.0f, 2.0f})
                       .add<Velocity>({3.0f, 4.0f});

    world.dispatch();

    unset_p_called = false;
    unset_v_called = false;
    unset_pv_called = false;

    entity1.destroy();

    world.dispatch();

    ASSERT(unset_p_called);
    ASSERT(unset_v_called);
    ASSERT(unset_pv_called);
}