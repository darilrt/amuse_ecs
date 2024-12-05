#pragma once

#include <iostream>

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <algorithm>

#include "amuse_ecs/ecs.hpp"

class World;

struct ArchetypeId
{
    std::vector<ComponentId> ids;

    ArchetypeId copy() const;

    void add(ComponentId id);

    void remove(ComponentId id);

    bool contains(ComponentId id) const;

    bool contains(const ArchetypeId &other) const;

    inline ComponentId at(size_t index) const
    {
        return ids.at(index);
    }

    bool operator==(const ArchetypeId &other) const
    {
        return ids == other.ids;
    }
};

class Archetype
{
public:
    World &world;

    // Archetype id is a sorted list of component ids that are in the archetype
    ArchetypeId id;

    // This is contains the component local index in the component data array
    std::unordered_map<std::type_index, size_t> component_index;

    // List of entities that are in this archetype in the same order as the component data
    std::vector<EntityId> entities;

    // This is the table for the entities and the components
    // Let's say we have 3 components, the table will look like this:
    // | Component | Entity 0 | Entity 1 | Entity 2 | ... |
    // | Comp A    | Comp*    | null     | Comp*    | ... |
    // | Comp B    | null     | Comp*    | null     | ... |
    // | Comp C    | Comp*    | null     | null     | ... |
    std::vector<std::vector<void *>> component_data;

    Archetype(World &world) : world(world) {}

    void add_entity(EntityId entity_id);

    // Remove the entity from the archetype deleting the components
    void delete_entity(EntityId entity_id);

    // Extract the entity from the archetype without deleting the components
    void extract_entity(EntityId entity_id);

    template <typename T>
    void set_component(EntityId entity_id, const T &component)
    {
        auto it = component_index.find(std::type_index(typeid(T)));

        assert(it != component_index.end() && "Component does not exist in archetype");

        auto &component_vector = component_data[it->second];

        auto index = std::find(entities.begin(), entities.end(), entity_id) - entities.begin();

        component_vector[index] = new T(component);
    }

    template <typename T>
    T *get_component(EntityId entity_id)
    {
        auto it = component_index.find(std::type_index(typeid(T)));
        assert(it != component_index.end() && "Component does not exist in archetype");

        auto index = it->second;

        auto entity_it = std::find(entities.begin(), entities.end(), entity_id);
        assert(entity_it != entities.end() && "Entity does not exist in archetype");

        auto entity_index = entity_it - entities.begin();

        return static_cast<T *>(component_data[index][entity_index]);
    }

    friend std::ostream &operator<<(std::ostream &os, const Archetype &archetype)
    {
        os << "Archetype(id=[";
        for (auto id : archetype.id.ids)
            os << id << ", ";
        os << "], entities=" << archetype.entities.size() << ")";
        return os;
    }
};
