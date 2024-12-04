#pragma once

#include "amuse_ecs/ecs.hpp"
#include "amuse_ecs/world.hpp"

// Entity class is a wrapper around an entity id
class Entity
{
public:
    Entity(EntityId id, World &world);

    inline EntityId get_id() const { return id; }

    inline void destroy() { world.destroy_entity(id); }

    template <typename T>
    inline Entity add(const T &component)
    {
        world.add_component(id, component);
        return *this;
    }

    template <typename T>
    inline T *get()
    {
        return world.get_component<T>(id);
    }

    template <typename T>
    inline bool has()
    {
        return world.get_component<T>(id) != nullptr;
    }

    template <typename T>
    inline void remove()
    {
        world.remove_component<T>(id);
    }

private:
    EntityId id;
    World &world;
};
