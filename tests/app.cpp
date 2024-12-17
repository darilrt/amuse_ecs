#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "ecs.hpp"

struct ModuleA
{
    ModuleA(ecs::App &app)
    {
        app.register_component<Position>();
        app.register_component<Velocity>();

        // app.system<Position>()
        //     .on<ecs::Set>(
        //         []()
        //         {
        //             p.x = 1.0f;
        //             p.y = 2.0f;
        //         });
    }
};

TEST(AppImportModule, "Import module")
{
    ecs::App app;
    app.use<ModuleA>();

    auto &world = app.world();

    ASSERT(world.component_deleters.size() == 2);
}
