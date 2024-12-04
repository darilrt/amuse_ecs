#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>
#include <algorithm>
#include <typeindex>
#include <string>

#include "amuse_ecs/ecs.hpp"
#include "amuse_ecs/archetype.hpp"

class Entity;

struct EntityInfo
{
    EntityId id;
    Archetype *archetype;
    std::string name;
};

class World
{
public:
    World();

    Entity create_entity(const std::string &name = "");

    void destroy_entity(EntityId id);

    EntityId find_entity(EntityId id);

    template <typename T>
    void register_component()
    {
        const auto type = std::type_index(typeid(T));

        if (component_map.find(type) == component_map.end())
        {
            component_map[type] = component_count++;

            delete_component_map[component_count] = [](void *component)
            { delete static_cast<T *>(component); };
        }
    }

    template <typename T>
    void add_component(EntityId entity_id, [[maybe_unused]] const T &component)
    {
        assert(find_entity(entity_id) != INVALID_ENTITY && "Entity does not exist");

        auto component_id = find_component<T>();

        assert(component_id != INVALID_COMPONENT && "Component does not exist");

        // auto &component_vector = components[id];
        // component_vector[entity_id] = new T(component);
    }

    template <typename T>
    T *get_component(EntityId entity_id)
    {
        assert(find_entity(entity_id) != INVALID_ENTITY && "Entity does not exist");
        assert(find_component<T>() != INVALID_COMPONENT && "Component does not exist");

        [[maybe_unused]] auto id = find_component<T>();
        return nullptr;
        // auto &component_vector = components[id];

        // return static_cast<T *>(component_vector[entity_id]);
    }

    template <typename T>
    void remove_component(EntityId entity_id)
    {
        assert(find_entity(entity_id) != INVALID_ENTITY && "Entity does not exist");
        assert(find_component<T>() != INVALID_COMPONENT && "Component does not exist");

        [[maybe_unused]] auto id = find_component<T>();
        // auto &component_vector = components[id];
        // delete static_cast<T *>(component_vector[entity_id]);
        // component_vector[entity_id] = nullptr;
    }

    template <typename T>
    inline ComponentId find_component()
    {
        auto it = component_map.find(std::type_index(typeid(T)));

        if (it == component_map.end())
        {
            return INVALID_COMPONENT;
        }

        return it->second;
    }

private:
    std::vector<Archetype> archetypes;

    EntityId entity_count;
    std::vector<EntityId> free_entities;
    std::vector<EntityInfo> entity_info;
    ComponentId component_count;
    std::unordered_map<std::type_index, ComponentId> component_map;
    std::unordered_map<ComponentId, void (*)(void *)> delete_component_map;

    template <typename... Components>
    std::vector<ComponentId> generate_archetype_id()
    {
        std::vector<ComponentId> id;
        (id.push_back(find_component<Components>()), ...);
        std::sort(id.begin(), id.end());
        return id;
    }

    inline void delete_component(ComponentId id, void *component)
    {
        delete_component_map[id](component);
    }

    template <typename... Components>
    Archetype &get_archetype()
    {
        auto id = generate_archetype_id<Components...>();

        for (auto &archetype : archetypes)
        {
            if (archetype.id == id)
            {
                return archetype;
            }
        }

        archetypes.push_back(Archetype(*this));
        auto &archetype = archetypes.back();
        archetype.id = id;

        return archetype;
    }

    inline ArchetypeId get_entity_archetype(EntityId id)
    {
        return entity_info[id].archetype->id;
    }

    friend class Archetype;
};
