#include "amuse_ecs/view.hpp"
#include "amuse_ecs/world.hpp"
#include "amuse_ecs/entity.hpp"

Entity ecs_get_entity(EntityId id, World &world)
{
    return Entity(id, world);
}