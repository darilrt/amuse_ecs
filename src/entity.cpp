#include "amuse_ecs/entity.hpp"
#include "amuse_ecs/world.hpp"

namespace ecs
{
    Entity::Entity(World &world, EntityId id, EntityMeta *meta = nullptr)
        : _id(id), _world(&world), _meta(meta)
    {
    }

    void Entity::destroy()
    {
        _world->destroy_entity(_id);
    }

    Entity Entity::add(ComponentId component_id, void *data)
    {
        _world->add_component(_id, component_id, data);
        return *this;
    }

    void *Entity::get(ComponentId component_id) const
    {
        if (_meta == nullptr)
        {
            return _world->get_component(_id, component_id);
        }
        return _meta->archetype == nullptr ? _world->get_component(_id, component_id) : _meta->archetype->get_component(_id, component_id);
    }

    void Entity::remove(ComponentId component_id)
    {
        _world->remove_component(_id, component_id);
    }

    Entity &Entity::operator=(const ecs::Entity &e)
    {
        _id = e._id;
        _world = e._world;
        _meta = e._meta;
        return *this;
    }
} // namespace ecs