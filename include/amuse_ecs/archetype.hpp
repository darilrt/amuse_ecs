#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <algorithm>

#include "amuse_ecs/ecs.hpp"

class World;

struct ArchetypeId
{
    std::vector<ComponentId> ids;

    ArchetypeId copy() const
    {
        ArchetypeId copy;
        copy.ids = ids;
        return copy;
    }

    void add(ComponentId id)
    {
        ids.push_back(id);
        std::sort(ids.begin(), ids.end());
    }

    bool contains(ComponentId id) const
    {
        return std::find(ids.begin(), ids.end(), id) != ids.end();
    }

    bool contains(const ArchetypeId &other) const
    {
        for (auto id : other.ids)
        {
            if (!contains(id))
            {
                return false;
            }
        }

        return true;
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

    void remove_entity(EntityId entity_id);

private:
};
