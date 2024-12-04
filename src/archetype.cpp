#include <cassert>

#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/world.hpp"

void Archetype::add_entity(EntityId entity_id)
{
    auto it = std::find(entities.begin(), entities.end(), entity_id);

    if (it == entities.end())
    {
        entities.push_back(entity_id);

        for (size_t i = 0; i < component_data.size(); i++)
        {
            component_data[i].push_back(nullptr);
        }

        return;
    }

    auto index = it - entities.begin();

    for (auto &component : component_data)
    {
        component[index] = nullptr;
    }
}

void Archetype::remove_entity(EntityId entity_id)
{
    // Find the entity in the archetype
    auto it = std::find(entities.begin(), entities.end(), entity_id);

    assert(it != entities.end() && "Entity does not exist in archetype");

    auto index = it - entities.begin();

    // Delete the components for this entity
    for (auto &component : component_data)
    {
        // world.delete_component(id[index], component[index]);
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