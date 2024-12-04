#include <amuse_utest/utest.hpp>

#define ECS_IMPLEMENTATION
#include "amuse_ecs/world.hpp"
#include "amuse_ecs/entity.hpp"
#include "amuse_ecs/archetype.hpp"

struct Position
{
    float x;
    float y;
};

struct Velocity
{
    float x;
    float y;
};

TEST(EntityCreation, "Entity creation")
{
    World world;

    Entity entity0 = world.create_entity();
    Entity entity1 = world.create_entity();

    ASSERT(entity0.id() == 0);
    ASSERT(entity1.id() == 1);
}

TEST(ComponentRegistration, "Component registration")
{
    World world;
    world.register_component<Position>();

    ASSERT(world.find_component<Position>() == 0);
}

TEST(FindEntity, "Find entity")
{
    World world;

    Entity entity0 = world.create_entity();
    Entity entity1 = world.create_entity();
    Entity entity2 = world.create_entity();

    entity1.destroy();

    ASSERT(world.find_entity(entity0.id()) != INVALID_ENTITY);
    ASSERT(world.find_entity(entity1.id()) == INVALID_ENTITY);
    ASSERT(world.find_entity(entity2.id()) != INVALID_ENTITY);
}

TEST(ArchetypeAddentityAndRemove, "Archetype add entity and remove")
{
    World world;
    Archetype archetype(world);

    EntityId entity0 = 0;
    EntityId entity1 = 1;
    EntityId entity2 = 2;

    archetype.add_entity(entity0);
    archetype.add_entity(entity1);
    archetype.add_entity(entity2);

    ASSERT(archetype.entities.size() == 3);

    archetype.remove_entity(entity1);

    ASSERT(archetype.entities.size() == 2);

    ASSERT(archetype.entities[0] == entity0);
    ASSERT(archetype.entities[1] == entity2);
}

TEST(ComponentAddition, "Component addition")
{
    World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    Entity entity = world.create_entity();
    entity.add<Position>({1.0f, 2.0f});
    entity.add<Velocity>({0.0f, 0.0f});

    Position *position = entity.get<Position>();
    Velocity *velocity = entity.get<Velocity>();

    ASSERT(position->x == 1.0f);
    ASSERT(position->y == 2.0f);

    ASSERT(velocity->x == 0.0f);
    ASSERT(velocity->y == 0.0f);
}

// TEST(ComponentRemoval, "Component removal")
// {
//     World world;
//     world.register_component<Position>();

//     Entity entity = world.create_entity();
//     entity.add<Position>({1.0f, 2.0f});
//     entity.remove<Position>();

//     ASSERT(!entity.has<Position>());
// }
