#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "amuse_ecs/world.hpp"
#include "amuse_ecs/archetype.hpp"

TEST(ArchetypeCreation, "Archetype creation")
{
    World world;

    Archetype &archetype_a = world.get_archetype<CompA>();
    Archetype &archetype_ab = world.get_archetype<CompA, CompB>();
    Archetype &archetype_ba = world.get_archetype<CompB, CompA>();
    Archetype &archetype_b = world.get_archetype<CompB>();

    // Archetype <CompA, CompB> should be the same as <CompB, CompA>
    ASSERT(archetype_ab.id == archetype_ba.id);

    // Validate that the archetypes contain the correct components
    ASSERT(archetype_a.id.contains<CompA>());
    ASSERT(!archetype_a.id.contains<CompB>());

    ASSERT(archetype_ab.id.contains<CompA>());
    ASSERT(archetype_ab.id.contains<CompB>());

    ASSERT(archetype_b.id.contains<CompB>());
    ASSERT(!archetype_b.id.contains<CompA>());
}

TEST(ArchetypeAddEntitySetGetComponent, "Archetype add entity, set and get component")
{
    World world;

    Archetype &archetype = world.get_archetype<CompA>();

    const int num_entities = 10;

    for (int i = 0; i < num_entities; i++)
    {
        archetype.add_entity(i);
        archetype.set_component(i, ECS_ID(CompA), new CompA{1.0f, 2.0f});
    }

    for (int i = 0; i < num_entities; i++)
    {
        CompA *comp_a = static_cast<CompA *>(archetype.get_component(i, ECS_ID(CompA)));

        ASSERT(comp_a->x == 1.0f);
        ASSERT(comp_a->y == 2.0f);
    }
}

TEST(ArchetypeDeleteEntity, "Archetype delete entity")
{
    World world;

    Archetype &archetype = world.get_archetype<CompA>();

    const int num_entities = 10;

    for (int i = 0; i < num_entities; i++)
    {
        archetype.add_entity(i);
        archetype.set_component(i, ECS_ID(CompA), new CompA{1.0f, 2.0f});
    }

    for (int i = 0; i < num_entities; i++)
    {
        CompA *comp_a = static_cast<CompA *>(archetype.get_component(i, ECS_ID(CompA)));
        delete comp_a;
        archetype.remve_entity(i);
    }

    ASSERT(archetype.entities.size() == 0);
}

TEST(ArchetypeMoveEntity, "Archetype move entities between archetypes")
{
    World world;

    Archetype &archetype_a = world.get_archetype<CompA>();
    Archetype &archetype_b = world.get_archetype<CompA, CompB>();
    [[maybe_unused]] Archetype &archetype_c = world.get_archetype<CompB>();

    const int num_entities = 10;

    for (int i = 0; i < num_entities; i++)
    {
        archetype_a.add_entity(i);
        archetype_a.set_component(i, ECS_ID(CompA), new CompA{1.0f, 2.0f});
    }

    for (int i = 0; i < num_entities; i++)
    {
        archetype_a.move_entity(archetype_b, i);
    }

    ASSERT(archetype_a.entities.size() == 0);
    ASSERT(archetype_b.entities.size() == num_entities);

    for (int i = 0; i < num_entities; i++)
    {
        CompA *comp_a = static_cast<CompA *>(archetype_b.get_component(i, ECS_ID(CompA)));

        ASSERT(comp_a->x == 1.0f);
        ASSERT(comp_a->y == 2.0f);
    }
}