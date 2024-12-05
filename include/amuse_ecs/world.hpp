#pragma once

#include <iostream>

#include <vector>
#include <unordered_map>
#include <cassert>
#include <algorithm>
#include <typeindex>
#include <string>
#include <memory>

#include "amuse_ecs/ecs.hpp"
#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/view.hpp"

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
            component_map[type] = component_count;
            delete_component_map[component_count] = [](void *component)
            { delete static_cast<T *>(component); };

            component_count++;
        }
    }

    template <typename T>
    void add_component(EntityId entity_id, [[maybe_unused]] const T &component)
    {
        assert(find_entity(entity_id) != INVALID_ENTITY && "Entity does not exist");

        auto component_id = find_component<T>();

        assert(component_id != INVALID_COMPONENT && "Component does not exist");

        auto &archetype = *entity_info[entity_id].archetype;

        auto new_archetype_id = archetype.id.copy();
        new_archetype_id.add(component_id);

        auto &new_archetype = get_archetype_by_id(new_archetype_id);
        new_archetype.add_entity(entity_id);

        // Copy the components from the old archetype to the new archetype
        for (auto index : archetype.component_index)
        {
            auto new_index = new_archetype.component_index[index.first];

            new_archetype.component_data[new_index][entity_id] = archetype.component_data[index.second][entity_id];
        }

        new_archetype.set_component<T>(entity_id, component);
        entity_info[entity_id].archetype = &new_archetype;

        archetype.extract_entity(entity_id);
    }

    template <typename T>
    T *get_component(EntityId entity_id)
    {
        assert(find_entity(entity_id) != INVALID_ENTITY && "Entity does not exist");

        auto component_id = find_component<T>();
        assert(component_id != INVALID_COMPONENT && "Component does not exist");

        auto &archetype = *entity_info[entity_id].archetype;

        if (archetype.id.contains(component_id))
        {
            return archetype.get_component<T>(entity_id);
        }

        return nullptr;
    }

    template <typename T>
    void remove_component(EntityId entity_id)
    {
        assert(find_entity(entity_id) != INVALID_ENTITY && "Entity does not exist");

        auto component_id = find_component<T>();
        assert(component_id != INVALID_COMPONENT && "Component does not exist");

        auto &old_archetype = *entity_info[entity_id].archetype;

        auto new_archetype_id = old_archetype.id.copy();
        new_archetype_id.remove(component_id);

        auto &new_archetype = get_archetype_by_id(new_archetype_id);
        new_archetype.add_entity(entity_id);

        // // Copy the components from the old archetype to the new archetype except the component to be removed
        for (auto index : new_archetype.component_index)
        {
            auto old_index = old_archetype.component_index[index.first];

            new_archetype.component_data[index.second][entity_id] = old_archetype.component_data[old_index][entity_id];
        }

        // Delete the component that was not copied
        const std::type_index type = std::type_index(typeid(T));
        void *component = old_archetype.component_data[old_archetype.component_index[type]][entity_id];
        delete_component(component_id, component);

        // Update the archetype of the entity
        entity_info[entity_id].archetype = &new_archetype;

        // Extract the entity from the old archetype
        old_archetype.extract_entity(entity_id);
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

    Archetype &get_archetype_by_id(ArchetypeId id)
    {
        // Find the archetype with the same id if it exists
        for (auto &archetype : archetypes)
        {
            if (archetype->id == id)
            {
                return *archetype;
            }
        }

        assert(id.ids.size() > 0 && "Archetype id is empty");

        // Else create a new archetype
        archetypes.push_back(std::make_unique<Archetype>(*this));
        auto &archetype = *archetypes.back();
        archetype.id = id;

        for (auto component_id : id.ids)
        {
            // Find the type index of the component
            auto it = std::find_if(component_map.begin(), component_map.end(), [component_id](const auto &pair)
                                   { return pair.second == component_id; });

            assert(it != component_map.end() && "Component does not exist");

            // Generate the component index
            archetype.component_index[it->first] = archetype.component_data.size();
            archetype.component_data.push_back({});
        }

        return archetype;
    }

    template <typename... Components>
    inline Archetype &get_archetype()
    {
        return get_archetype_by_id(generate_archetype_id<Components...>());
    }

    template <typename... Components>
    View<Components...> view()
    {
        View<Components...> view;

        for (auto &archetype : archetypes)
        {
            if (archetype->id.contains(generate_archetype_id<Components...>()))
            {
                view.archetypes.push_back(archetype.get());
            }
        }

        return view;
    }

    inline EntityInfo &get_entity_info(EntityId id)
    {
        return entity_info[id];
    }

private:
    std::vector<std::unique_ptr<Archetype>> archetypes;

    EntityId entity_count;
    std::vector<EntityId> free_entities;
    std::vector<EntityInfo> entity_info;
    ComponentId component_count;
    std::unordered_map<std::type_index, ComponentId> component_map;
    std::unordered_map<ComponentId, void (*)(void *)> delete_component_map;

    template <typename... Components>
    ArchetypeId generate_archetype_id()
    {
        std::vector<ComponentId> id;
        (id.push_back(find_component<Components>()), ...);
        std::sort(id.begin(), id.end());

        ArchetypeId archetype_id;
        archetype_id.ids = id;
        return archetype_id;
    }

    inline void delete_component(ComponentId id, void *component)
    {
        delete_component_map[id](component);
    }

    friend class Archetype;
};
