#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "ecs.hpp"

struct ModuleA
{
    ModuleA(ecs::App &app)
    {
        app.register_component<Position>();
        app.register_component<Velocity>();
    }
};

TEST(AppImportModule, "Import module")
{
    ecs::App app;
    app.use<ModuleA>();

    auto &world = app.world();

    ASSERT(world.component_deleters.size() == 2);
}

TEST(AppEvent, "Event handling")
{
    ecs::App app;
    app.use<ModuleA>();

    bool called_position_velocity_on_set = false;
    bool called_position_velocity_on_update = false;

    app.system<Position, Velocity>()
        .on<ecs::Set>([&]([[maybe_unused]] ecs::App &app, [[maybe_unused]] Position &p, [[maybe_unused]] Velocity &v)
                      { called_position_velocity_on_set = true; })
        .on<ecs::Update>(
            [&]([[maybe_unused]] ecs::App &app, [[maybe_unused]] Position &p, [[maybe_unused]] Velocity &v)
            {
                called_position_velocity_on_update = true;
            });

    app.world().entity().add<Position>({1.0f, 2.0f}).add<Velocity>({3.0f, 4.0f});

    app.world().dispatch();

    app.event_handler().emit<ecs::Update>();

    ASSERT(called_position_velocity_on_set);
    ASSERT(called_position_velocity_on_update);
}