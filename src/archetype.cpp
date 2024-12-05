#include <cassert>

#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/world.hpp"

ArchetypeId ArchetypeId::copy() const
{
    ArchetypeId copy;
    copy.ids = ids;
    return copy;
}

void ArchetypeId::add(ComponentId id)
{
    if (std::find(ids.begin(), ids.end(), id) == ids.end())
    {
        ids.push_back(id);
        std::sort(ids.begin(), ids.end());
    }
}

void ArchetypeId::remove(ComponentId id)
{
    auto it = std::find(ids.begin(), ids.end(), id);

    if (it != ids.end())
    {
        ids.erase(it);
    }

    std::sort(ids.begin(), ids.end());
}

bool ArchetypeId::contains(ComponentId id) const
{
    return std::find(ids.begin(), ids.end(), id) != ids.end();
}

bool ArchetypeId::contains(const ArchetypeId &other) const
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

void Archetype::add_entity(EntityId entity_id)
{
    auto it = std::find(entities.begin(), entities.end(), entity_id);

    assert(it == entities.end() && "Entity already exists in archetype");

    entities.push_back(entity_id);

    for (size_t i = 0; i < component_data.size(); i++)
    {
        component_data[i].push_back(nullptr);
    }
}

void Archetype::delete_entity(EntityId entity_id)
{
    // Find the entity in the archetype
    auto it = std::find(entities.begin(), entities.end(), entity_id);

    assert(it != entities.end() && "Entity does not exist in archetype");

    auto index = it - entities.begin();

    // Delete the components for this entity
    for (auto &component : component_data)
    {
        world.delete_component(id.at(index), component[index]);
        component[index] = nullptr;
    }

    // If the entity is not the last one, swap it with the last one
    if (index != entities.size() - 1)
    {
        for (size_t i = 0; i < component_data.size(); i++)
        {
            component_data[i][index] = component_data[i].back();
            component_data[i].pop_back();
        }

        entities[index] = entities.back();
        entities.pop_back();
    }
    else
    {
        for (auto &component : component_data)
        {
            component.pop_back();
        }

        entities.pop_back();
    }
}

void Archetype::extract_entity(EntityId entity_id)
{
    // Find the entity in the archetype
    auto it = std::find(entities.begin(), entities.end(), entity_id);

    assert(it != entities.end() && "Entity does not exist in archetype");

    auto index = it - entities.begin();

    // If the entity is not the last one, swap it with the last one
    if (index != entities.size() - 1)
    {
        for (size_t i = 0; i < component_data.size(); i++)
        {
            component_data[i][index] = component_data[i].back();
            component_data[i].pop_back();
        }

        entities[index] = entities.back();
        entities.pop_back();
    }
    else
    {
        for (auto &component : component_data)
        {
            component.pop_back();
        }

        entities.pop_back();
    }
}