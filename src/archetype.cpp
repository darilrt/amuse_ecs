#include <cassert>

#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/archetype_id.hpp"
#include "amuse_ecs/world.hpp"

Archetype::Archetype(World &world, const ArchetypeId &id)
    : id(id), world(world)
{
    // Check if archetype already exists
    for (size_t i = 0; i < world.archetypes.size(); i++)
    {
        assert(world.archetypes[i]->id != id && "Archetype already exists");
    }

    // Initialize component data
    for (auto comp_id : id.ids)
    {
        components[comp_id] = std::vector<void *>();
    }
}

void Archetype::add_entity(EntityId entity_id)
{
    entities.push_back(entity_id);

    for (auto &[comp_id, comp_data] : components)
    {
        comp_data.resize(entities.size());
        comp_data.back() = nullptr;
    }
}

void Archetype::remove_entity(EntityId entity_id)
{
    auto entity_index = get_entity_index(entity_id);

    assert(entity_index != -1 && "Entity does not exist in archetype");

    // Swap entity with last entity
    entities[entity_index] = entities.back();

    // Remove last entity
    entities.pop_back();

    for (auto &[comp_id, comp_data] : components)
    {
        // Swap component with last component
        comp_data[entity_index] = comp_data.back();

        // Remove last component
        comp_data.pop_back();
    }
}

size_t Archetype::get_entity_index(EntityId entity_id)
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (entities[i] == entity_id)
        {
            return i;
        }
    }

    return -1;
}

void Archetype::set_component(EntityId entity_id, ComponentId comp_id, void *component)
{
    assert(id.contains(comp_id) && "Archetype does not contain component");

    auto entity_index = get_entity_index(entity_id);

    assert(entity_index != -1 && "Entity does not exist in archetype");

    components[comp_id][entity_index] = component;
}

void *Archetype::get_component(EntityId entity_id, ComponentId comp_id)
{
    if (!id.contains(comp_id))
    {
        return nullptr;
    }

    auto entity_index = get_entity_index(entity_id);

    assert(entity_index != -1 && "Entity does not exist in archetype");

    return components[comp_id][entity_index];
}

void Archetype::move_entity(Archetype &new_archetype, EntityId entity_id)
{
    // Find entity index
    auto entity_index = get_entity_index(entity_id);

    assert(entity_index != -1 && "Entity does not exist in archetype");

    // Add entity to new archetype
    new_archetype.add_entity(entity_id);

    // Move components to new archetype
    for (auto &[comp_id, comp_data] : components)
    {
        // if component exists in new archetype move else delete
        if (new_archetype.id.contains(comp_id))
        {
            new_archetype.set_component(entity_id, comp_id, comp_data[entity_index]);
        }
        else
        {
            world.delete_component(comp_id, comp_data[entity_index]);
        }
    }

    // Remove entity from current archetype
    remove_entity(entity_id);
}

std::ostream &operator<<(std::ostream &os, const Archetype &archetype)
{
    os << "Archetype(" << archetype.id << ")" << std::endl;

    // print a table like:
    // Entity | CompA | CompB | CompC
    // 0      | 0x123 | 0x456 | 0x789
    // 1      | 0x123 | 0x456 | 0x789
    // 2      | 0x123 | 0x456 | 0x789
    // ...
    os << "Entity | ";

    for (auto &[comp_id, comp_data] : archetype.components)
    {
        os << comp_id.name() << " | ";
    }

    os << std::endl;

    for (size_t i = 0; i < archetype.entities.size(); i++)
    {
        os << i << " | ";

        for (auto &[comp_id, comp_data] : archetype.components)
        {
            os << comp_data[i] << " | ";
        }

        os << std::endl;
    }

    return os;
}