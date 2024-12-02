#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

typedef uint64_t EcsId;
typedef EcsId EntityId;
typedef EcsId ComponentId;

class Entity;

class World
{
public:
    World();

    Entity create_entity();

    void destroy_entity(EntityId id);

    template <typename T>
    void register_component()
    {
        if (component_map.find(typeid(T).name()) == component_map.end())
        {
            component_map[typeid(T).name()] = component_count++;
            resize_components();
        }
    }

    template <typename T>
    void add_component(EntityId entity_id, const T &component)
    {
        auto it = component_map.find(typeid(T).name());

        if (it == component_map.end())
        {
            register_component<T>();
            it = component_map.find(typeid(T).name());
        }

        auto &component_vector = components[it->second];
        component_vector[entity_id] = allocate_component<T>(component);
    }

    template <typename T>
    T *get_component(EntityId entity_id)
    {
        auto it = component_map.find(typeid(T).name());

        if (it == component_map.end())
        {
            return nullptr;
        }

        auto &component_vector = components[it->second];
        return static_cast<T *>(component_vector[entity_id]);
    }

    template <typename T>
    void remove_component(EntityId entity_id)
    {
        auto it = component_map.find(typeid(T).name());

        if (it == component_map.end())
        {
            return;
        }

        auto &component_vector = components[it->second];
        delete static_cast<T *>(component_vector[entity_id]);
        component_vector[entity_id] = nullptr;
    }

private:
    struct ComponentData
    {
        void *data;
        size_t size;
    };

    // Layout
    // |        | Entity 0 | Entity 1 | Entity 2 | ... |
    // | Comp A | Comp*    | null     | Comp*    | ... |
    // | Comp B | null     | Comp*    | null     | ... |
    // | Comp C | Comp*    | null     | null     | ... |
    // | ...    | ...      | ...      | ...      | ... |
    std::vector<std::vector<void *>> components;
    EntityId entity_count;
    std::vector<EntityId> free_entities;
    ComponentId component_count;
    std::unordered_map<std::string, ComponentId> component_map;
    std::vector<ComponentData> component_data;

    void resize_components();

    template <typename T>
    void *allocate_component(const T &component)
    {
        T *data = new T(component);
        component_data.push_back({data, sizeof(T)});
        return data;
    }
};

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

#ifdef ECS_IMPLEMENTATION

Entity::Entity(EntityId id, World &world) : world(world)
{
    this->id = id;
}

World::World()
{
    entity_count = 0;
    component_count = 0;
}

Entity World::create_entity()
{
    EntityId id;

    if (free_entities.empty())
    {
        id = entity_count++;
        resize_components();
    }
    else
    {
        id = free_entities.back();
        free_entities.pop_back();
    }

    return Entity(id, *this);
}

void World::destroy_entity(EntityId id)
{
    free_entities.push_back(id);
}

void World::resize_components()
{
    components.resize(component_count);
    for (auto &component : components)
    {
        uint64_t size = component.size();
        component.resize(entity_count);

        for (uint64_t i = size; i < entity_count; i++)
        {
            component[i] = nullptr;
        }
    }
}

#endif // ECS_IMPLEMENTATION