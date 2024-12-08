#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "amuse_ecs/archetype_id.hpp"

TEST(ArchetypeIdMethods, "Test ArchetypeId methods")
{
    ArchetypeId id0;

    id0.add(typeid(CompA));
    id0.add(typeid(CompB));

    ArchetypeId id0eq;
    id0eq.add(typeid(CompB));
    id0eq.add(typeid(CompA));

    // Check if the ids are equal even if the order is different
    ASSERT(id0 == id0eq);

    // Check if the ids contain the correct components
    ASSERT(id0.contains(typeid(CompA)));
    ASSERT(id0.contains(typeid(CompB)));

    ArchetypeId id1;
    id1.add(typeid(CompA));

    // Check if the ids are not equal
    ASSERT(id0 != id1);

    // Check if the ids contain the correct components
    ASSERT(id1.contains(typeid(CompA)));

    // Check if the ids do not contain the correct components
    ASSERT(!id1.contains(typeid(CompB)));

    // Check if id0 contains id1
    ASSERT(id0.contains(id1));
}