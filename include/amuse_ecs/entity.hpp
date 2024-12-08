#pragma once

#include "amuse_ecs/ecs.hpp"
#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/entity_info.hpp"

class World;

// Entity class is a wrapper around an entity id
class Entity
{
public:
    Entity(World &world, EntityId id, EntityMeta *meta);

    inline EntityId id() const { return _id; }

    inline World &world() { return _world; }

    inline EntityMeta &meta() { return *_meta; }

    inline const World &world() const { return _world; }

    void destroy();

    Entity add(ComponentId component_id, void *data);

    template <typename T>
    inline Entity add(const T &component)
    {
        return add(ECS_ID(T), new T(component));
    }

    void *get(ComponentId component_id) const;

    template <typename T>
    inline T *get() const
    {
        return static_cast<T *>(get(ECS_ID(T)));
    }

    template <typename T>
    inline bool has() const
    {
        return get<T>() != nullptr;
    }

    void remove(ComponentId component_id);

    template <typename T>
    inline void remove()
    {
        remove(ECS_ID(T));
    }

private:
    EntityId _id;
    World &_world;
    EntityMeta *_meta;
};
