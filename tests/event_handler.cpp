#include <amuse_utest/utest.hpp>

#include "amuse_ecs/event_handler.hpp"

struct EventA
{
};

struct EventB
{
};

TEST(EventHandler, "Event handler")
{
    ecs::EventHandler handler;

    int a = 0;
    int b = 0;

    handler.on<EventA>([&a]()
                       { a++; });

    handler.on<EventA>([&a]()
                       { a++; });

    handler.on<EventB>([&b]()
                       { b++; });

    handler.on<EventB>([&b]()
                       { b++; });

    handler.emit<EventA>();
    handler.emit<EventB>();

    ASSERT(a == 2);
    ASSERT(b == 2);
}