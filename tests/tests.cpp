#include <amuse_utest/utest.hpp>

#define ECS_IMPLEMENTATION
#include "amuse_ecs/world.hpp"
#include "amuse_ecs/entity.hpp"

struct Position
{
    float x;
    float y;
};

TEST(EntityCreation, "Entity creation")
{
    World world;

    Entity entity0 = world.create_entity();
    Entity entity1 = world.create_entity();

    ASSERT(entity0.get_id() == 0);
    ASSERT(entity1.get_id() == 1);
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

    ASSERT(world.find_entity(entity0.get_id()) != INVALID_ENTITY);
    ASSERT(world.find_entity(entity1.get_id()) == INVALID_ENTITY);
    ASSERT(world.find_entity(entity2.get_id()) != INVALID_ENTITY);
}

// TEST(ComponentAddition, "Component addition")
// {
//     World world;
//     world.register_component<Position>();

//     Entity entity = world.create_entity();
//     entity.add<Position>({1.0f, 2.0f});

//     Position *position = entity.get<Position>();

//     ASSERT(position->x == 1.0f);
//     ASSERT(position->y == 2.0f);
// }

// TEST(ComponentRemoval, "Component removal")
// {
//     World world;
//     world.register_component<Position>();

//     Entity entity = world.create_entity();
//     entity.add<Position>({1.0f, 2.0f});
//     entity.remove<Position>();

//     ASSERT(!entity.has<Position>());
// }
