#pragma once

#include "amuse_ecs/ecs.hpp"
#include "amuse_ecs/world.hpp"

// Entity class is a wrapper around an entity id
class Entity
{
public:
    Entity(EntityId id, World &world);

    inline EntityId id() const { return _id; }

    inline World &world() { return _world; }

    inline void destroy() { _world.destroy_entity(_id); }

    template <typename T>
    inline Entity add(const T &component)
    {
        _world.add_component(_id, component);
        return *this;
    }

    template <typename T>
    inline T *get()
    {
        return _world.get_component<T>(_id);
    }

    template <typename T>
    inline bool has()
    {
        return _world.get_component<T>(_id) != nullptr;
    }

    template <typename T>
    inline void remove()
    {
        _world.remove_component<T>(_id);
    }

private:
    EntityId _id;
    World &_world;
};
