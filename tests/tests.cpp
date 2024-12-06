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

    archetype.delete_entity(entity1);

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
    entity.add<Velocity>({3.0f, 4.0f});

    Position *position = entity.get<Position>();
    Velocity *velocity = entity.get<Velocity>();

    ASSERT(position->x == 1.0f);
    ASSERT(position->y == 2.0f);

    ASSERT(velocity->x == 3.0f);
    ASSERT(velocity->y == 4.0f);

    Archetype &archetype = world.get_archetype<Position, Velocity>();

    ASSERT(archetype.entities.size() == 1);

    Archetype &archetype_position = world.get_archetype<Position>();

    ASSERT(archetype_position.entities.size() == 0);
}

TEST(ComponentRemoval, "Component removal")
{
    World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    Entity entity = world.create_entity();
    entity.add<Position>({1.0f, 2.0f});
    entity.add<Velocity>({3.0f, 4.0f});

    entity.remove<Position>();

    ASSERT(!entity.has<Position>());

    entity.remove<Velocity>();

    ASSERT(!entity.has<Velocity>());
}

TEST(ViewIteration, "View iteration")
{
    World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    Entity entity0 = world.create_entity();
    entity0.add<Position>({1.0f, 2.0f});
    entity0.add<Velocity>({3.0f, 4.0f});

    Entity entity1 = world.create_entity();
    entity1.add<Position>({5.0f, 6.0f});

    auto view = world.view<Position>();

    ASSERT(view.archetypes.size() == 2);

    while (view.next())
    {
        auto &archetype = view.get();

        for (size_t i = 0; i < archetype.entities.size(); i++)
        {
            EntityId entity_id = archetype.entities[i];
            EntityInfo entity = world.get_entity_info(entity_id);

            Position *position = archetype.get_component<Position>(entity_id);

            if (entity_id == 0)
            {
                ASSERT(position->x == 1.0f);
                ASSERT(position->y == 2.0f);
            }
            else if (entity_id == 1)
            {
                ASSERT(position->x == 5.0f);
                ASSERT(position->y == 6.0f);
            }
        }
    }
}

TEST(ViewEach, "Use of each view function")
{
    World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    Entity entity0 = world.create_entity();
    entity0.add<Position>({1.0f, 2.0f});
    entity0.add<Velocity>({3.0f, 4.0f});

    Entity entity1 = world.create_entity();
    entity1.add<Position>({5.0f, 6.0f});

    auto view = world.view<Position, Velocity>();

    ASSERT(view.archetypes.size() == 1);
    ASSERT(view.archetypes[0]->entities.size() == 1);

    view.each([&_result](EntityId entity_id, Position *position, Velocity *velocity)
              {
                  if (entity_id == 0)
                  {
                      ASSERT(position->x == 1.0f);
                      ASSERT(position->y == 2.0f);

                      ASSERT(velocity->x == 3.0f);
                      ASSERT(velocity->y == 4.0f);
                  } });
}