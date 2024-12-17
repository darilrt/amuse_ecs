#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "amuse_ecs/archetype_id.hpp"

TEST(ArchetypeIdMethods, "Test ArchetypeId methods")
{
    ecs::ArchetypeId id0;

    id0.add(ECS_ID(CompA));
    id0.add(ECS_ID(CompB));

    ecs::ArchetypeId id0eq;
    id0eq.add(ECS_ID(CompB));
    id0eq.add(ECS_ID(CompA));

    // Check if the ids are equal even if the order is different
    ASSERT(id0 == id0eq);

    // Check if the ids contain the correct components
    ASSERT(id0.contains(ECS_ID(CompA)));
    ASSERT(id0.contains(ECS_ID(CompB)));

    ecs::ArchetypeId id1;
    id1.add(ECS_ID(CompA));

    // Check if the ids are not equal
    ASSERT(id0 != id1);

    // Check if the ids contain the correct components
    ASSERT(id1.contains(ECS_ID(CompA)));

    // Check if the ids do not contain the correct components
    ASSERT(!id1.contains(ECS_ID(CompB)));

    // Check if id0 contains id1
    ASSERT(id0.contains(id1));
}

TEST(ArchetypeIdFromIds, "Test ArchetypeId from_ids")
{
    ecs::ArchetypeId id0 = ecs::ArchetypeId::from_ids({ECS_ID(CompA), ECS_ID(CompB)});

    ecs::ArchetypeId id1 = ecs::ArchetypeId::from_ids({ECS_ID(CompA)});

    // Check if the ids contain the correct components
    ASSERT(id0.contains(ECS_ID(CompA)));
    ASSERT(id0.contains(ECS_ID(CompB)));

    // Check if the ids contain the correct components
    ASSERT(id1.contains(ECS_ID(CompA)));

    // Check if the ids do not contain the correct components
    ASSERT(!id1.contains(ECS_ID(CompB)));
}