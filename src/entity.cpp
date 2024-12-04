#include "amuse_ecs/entity.hpp"
#include "amuse_ecs/world.hpp"

Entity::Entity(EntityId id, World &world) : _world(world)
{
    this->_id = id;
}
