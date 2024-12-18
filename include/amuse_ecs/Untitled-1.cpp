#include <ecs.hpp>

struct CompA
{
};

struct ModuleA
{
    ModuleA(App &app)
    {
        app.register_component<CompA>();

        app.system<CompA>()
            .on<Update>(ModuleA::update);
    }

    static void update(CompA &comp)
    {
    }
};

int main(int argc, char **argv)
{

    App()
        .use<ModuleA>()
        .run();

    return 0;
}