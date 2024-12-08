#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

#include "amuse_ecs/ecs.hpp"
#include "amuse_ecs/archetype_id.hpp"

class World;

class Archetype
{
public:
    World &world;
    ArchetypeId id;

    std::vector<EntityId> entities;

    // Map of component ids to component data
    // Memory layout
    //  CompId | Entity 0 | Entity 1 | Entity 2 | ... |
    //       0 | Comp*    | Comp*    | Comp*    | ... |
    //       1 | Comp*    | Comp*    | Comp*    | ... |
    //       2 | Comp*    | Comp*    | Comp*    | ... |
    std::unordered_map<ComponentId, std::vector<void *>> components;

    Archetype(World &world, const ArchetypeId &id);

    void add_entity(EntityId entity_id);

    // Remove entity from archetype but not delete components from memory
    // this need to be handled externally
    void remve_entity(EntityId entity_id);

    size_t get_entity_index(EntityId entity_id);

    void set_component(EntityId entity_id, ComponentId id, void *component);

    void *get_component(EntityId entity_id, ComponentId id);

    void move_entity(Archetype &new_archetype, EntityId entity_id);

    friend std::ostream &operator<<(std::ostream &os, const Archetype &archetype);
};