#include <cassert>
#include <algorithm>

#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/archetype_id.hpp"
#include "amuse_ecs/world.hpp"
#include "amuse_ecs/entity.hpp"

namespace ecs
{
    Archetype::Archetype(World &world, const ArchetypeId &id)
        : id(id), world(world)
    {
        // Check if archetype already exists
        for (size_t i = 0; i < world.archetypes.size(); i++)
        {
            assert(world.archetypes[i]->id != id && "Archetype already exists");
        }

        // Resize component data
        ComponentId max_id;

        if (id.size() == 0)
        {
            max_id = -1;
        }
        else
        {
            max_id = *std::max_element(id.begin(), id.end());
        }

        component_data.resize(max_id + 1);

        // Initialize component data
        for (auto comp_id : id)
        {
            component_data[comp_id] = std::vector<void *>();
        }
    }

    void Archetype::add_entity(EntityId entity_id)
    {
        entities.push_back(entity_id);

        for (auto comp_id : id)
        {
            component_data[comp_id].push_back(nullptr);
        }
    }

    void Archetype::remove_entity(EntityId entity_id)
    {
        auto entity_index = get_entity_index(entity_id);

        assert(entity_index != -1 && "Entity does not exist in archetype");

        entities.erase(entities.begin() + entity_index);

        for (auto comp_id : id)
        {
            auto comp_data = component_data[comp_id];
            comp_data.erase(comp_data.begin() + entity_index);
        }

        // // Swap entity with last entity
        // entities[entity_index] = entities.back();

        // // Remove last entity
        // entities.pop_back();

        // // Remove entity from component data
        // for (auto comp_id : id)
        // {
        //     auto comp_data = component_data[comp_id];

        //     // Swap component with last component
        //     comp_data[entity_index] = comp_data.back();

        //     // Remove last component
        //     comp_data.pop_back();
        // }
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

        component_data[comp_id][entity_index] = component;
    }

    void *Archetype::get_component(EntityId entity_id, ComponentId comp_id)
    {
        if (!id.contains(comp_id))
        {
            return nullptr;
        }

        auto entity_index = get_entity_index(entity_id);

        assert(entity_index != -1 && "Entity does not exist in archetype");

        return component_data[comp_id][entity_index];
    }

    void Archetype::move_entity(Archetype &new_archetype, EntityId entity_id)
    {
        // Find entity index
        auto entity_index = get_entity_index(entity_id);

        assert(entity_index != -1 && "Entity does not exist in archetype");

        // Add entity to new archetype
        new_archetype.add_entity(entity_id);

        // Move components to new archetype
        for (auto comp_id : id)
        {
            auto comp_data = component_data[comp_id];

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

    Entity Archetype::find(EntityId entity_id)
    {
        return world.find(entity_id);
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

        for (auto comp_id : archetype.id)
        {
            os << comp_id << " | ";
        }

        os << std::endl;

        for (size_t i = 0; i < archetype.entities.size(); i++)
        {
            os << i << " | ";

            for (auto comp_id : archetype.id)
            {
                auto comp_data = archetype.component_data[comp_id];
                os << comp_data[i] << " | ";
            }

            os << std::endl;
        }

        return os;
    }
} // namespace ecs