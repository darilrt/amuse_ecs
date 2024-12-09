#include <amuse_utest/utest.hpp>
#include <string>

#include "components.hpp"
#include "amuse_ecs/world.hpp"
#include "amuse_ecs/entity.hpp"

TEST(EntityCreation, "Entity creation")
{
    World world;

    [[maybe_unused]] Entity entity0 = world.entity("Entity 0");

    ASSERT(world.entities.size() == 0);

    world.dispatch();

    ASSERT(world.entities.size() == 1);
    ASSERT(world.entities[0].name == "Entity 0");

    [[maybe_unused]] Entity entity1 = world.entity("Entity 1");

    world.dispatch();

    ASSERT(world.entities.size() == 2);
    ASSERT(world.entities[1].name == "Entity 1");
}

TEST(FindEntity, "Find entity")
{
    World world;

    const int entity_count = 20;
    for (int i = 0; i < entity_count; i++)
    {
        world.entity("Entity " + std::to_string(i));
    }

    world.dispatch();

    Entity found_entity = world.find("Entity 0");
    Entity not_found_entity = world.find("Entity 100");

    ASSERT(found_entity.id() != INVALID_ENTITY);
    ASSERT(not_found_entity.id() == INVALID_ENTITY);
}

TEST(EntityDestroy, "Entity destroy")
{
    World world;

    Entity entity0 = world.entity("Entity 0");
    Entity entity1 = world.entity("Entity 1");

    world.dispatch();

    ASSERT(world.find("Entity 0").id() != INVALID_ENTITY);
    ASSERT(world.find("Entity 1").id() != INVALID_ENTITY);

    entity0.destroy();
    world.dispatch();

    ASSERT(world.find("Entity 0").id() == INVALID_ENTITY);
    ASSERT(world.find("Entity 1").id() != INVALID_ENTITY);

    entity1.destroy();
    world.dispatch();

    ASSERT(world.find("Entity 0").id() == INVALID_ENTITY);
    ASSERT(world.find("Entity 1").id() == INVALID_ENTITY);
}
