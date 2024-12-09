#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "ecs.hpp"

TEST(ViewUse, "Use of a view")
{
    World world;
    world.register_component<Position>();
    world.register_component<Velocity>();

    const size_t entity_count = 10;
    for (size_t i = 0; i < entity_count; ++i)
    {
        world.entity()
            .add<Position>({1.0, 2.0})
            .add<Velocity>({(float)i, (float)i});

        world.entity()
            .add<Position>({1.0, 2.0});
    }

    world.dispatch();

    View<Position, Velocity> view = world.view<Position, Velocity>();

    ASSERT(view.archetypes.size() == 1);
    ASSERT(view.size() == entity_count);

    view.each(
        [&]([[maybe_unused]] Entity e, Position &p, [[maybe_unused]] Velocity &v)
        {
            ASSERT(p.x == 1.0);
            ASSERT(p.y == 2.0);
        });

    View<Position> view2 = world.view<Position>();

    ASSERT(view2.archetypes.size() == 2.0);
    ASSERT(view2.size() == entity_count * 2);

    view2.each(
        [&]([[maybe_unused]] Entity e, Position &p)
        {
            ASSERT(p.x == 1.0);
            ASSERT(p.y == 2.0);
        });
}