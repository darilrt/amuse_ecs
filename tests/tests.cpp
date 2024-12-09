#include <amuse_utest/utest.hpp>

#include "components.hpp"
#include "amuse_ecs/world.hpp"
#include "amuse_ecs/entity.hpp"
#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/view.hpp"

#include "archetype_id.cpp"
#include "archetype.cpp"
#include "entity.cpp"
#include "component.cpp"
#include "view.cpp"

// TEST(ViewIteration, "View iteration")
// {
//     World world;
//     world.register_component<Position>();
//     world.register_component<Velocity>();

//     Entity entity0 = world.create_entity();
//     entity0.add<Position>({1.0f, 2.0f});

//     Entity entity1 = world.create_entity();
//     entity1.add<Position>({5.0f, 6.0f});
//     entity1.add<Velocity>({3.0f, 4.0f});

//     View<Position> view(world);

//     ASSERT(view.archetypes.size() == 2);

//     while (view.next())
//     {
//         auto &archetype = view.get();

//         for (size_t i = 0; i < archetype.entities.size(); i++)
//         {
//             EntityId entity_id = archetype.entities[i];
//             EntityInfo entity = world.get_entity_info(entity_id);

//             Position *position = archetype.get_component<Position>(entity_id);

//             if (entity_id == 0)
//             {
//                 ASSERT(position->x == 1.0f);
//                 ASSERT(position->y == 2.0f);
//             }
//             else if (entity_id == 1)
//             {
//                 ASSERT(position->x == 5.0f);
//                 ASSERT(position->y == 6.0f);
//             }
//         }
//     }
// }

// TEST(ViewEach, "Use of each view function")
// {
//     World world;
//     world.register_component<Position>();
//     world.register_component<Velocity>();

//     Entity entity0 = world.create_entity();
//     entity0.add<Position>({1.0f, 2.0f});
//     entity0.add<Velocity>({3.0f, 4.0f});

//     Entity entity1 = world.create_entity();
//     entity1.add<Position>({5.0f, 6.0f});

//     View<Position, Velocity> view(world);

//     ASSERT(view.archetypes.size() == 1);
//     ASSERT(view.archetypes[0]->entities.size() == 1);

//     view.each([&_result](Entity entity, Position *position, Velocity *velocity)
//               {
//                   auto p = entity.get<Position>();
//                   std::cout << p->x << " " << p->y << std::endl;
//                   if (entity.id() == 0)
//                   {
//                       ASSERT(position->x == 1.0f);
//                       ASSERT(position->y == 2.0f);

//                       ASSERT(velocity->x == 3.0f);
//                       ASSERT(velocity->y == 4.0f);
//                   } });
// }