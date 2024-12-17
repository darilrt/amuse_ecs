#include <stdexcept>

#include "amuse_ecs/world.hpp"
#include "amuse_ecs/entity.hpp"
#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/view.hpp"

namespace ecs
{
    World::World()
    {
        // Create an archetype with no components
        archetypes.push_back(std::make_unique<Archetype>(*this, ArchetypeId()));
    }

    Entity World::entity(const std::string &name)
    {
        EntityId id;

        // If there are free entities, use one of them
        // Otherwise, create a new entity id
        if (free_entities.size() > 0)
        {
            id = free_entities.back();
            free_entities.pop_back();
        }
        else
        {
            id = next_entity_id++;
        }

        // Create entity meta
        EntityMeta meta{
            .id = id,             // EntityId to be used
            .archetype = nullptr, // nullptr because it's not used yet
            .name = name          // Name of the entity
        };

        // Create action to create entity
        actions.push_back(ActionInfo{
            .type = ActionInfo::Type::CreateEntity, // Action type is to create entity
            .meta = meta,                           // EntityMeta created above
            .archetype_id = ArchetypeId(),          // ArchetypeId void because entity has no components yet
            .components = {},                       // No components because entity has no components yet
        });

        // Return entity
        return Entity(*this, id, &actions.back().meta);
    }

    Entity World::entity_with_components(const std::string &name, const std::vector<std::pair<ComponentId, void *>> &components)
    {
        EntityId id;

        // If there are free entities, use one of them
        // Otherwise, create a new entity id
        if (free_entities.size() > 0)
        {
            id = free_entities.back();
            free_entities.pop_back();
        }
        else
        {
            id = next_entity_id++;
        }

        // Create entity meta
        EntityMeta meta{
            .id = id,             // EntityId to be used
            .archetype = nullptr, // nullptr because it's not used yet
            .name = name          // Name of the entity
        };

        auto archetype_id = ArchetypeId();

        // Get the new archetype id
        for (const std::pair<ComponentId, void *> &component : components)
        {
            archetype_id.add(component.first);
        }

        // Create action to create entity
        actions.push_back(ActionInfo{
            .type = ActionInfo::Type::CreateEntity, // Action type is to create entity
            .meta = meta,                           // EntityMeta created above
            .archetype_id = archetype_id,           // ArchetypeId with components to be added to entity
            .components = components,               // Components to be added to entity
        });

        // Return entity
        return Entity(*this, id, &actions.back().meta);
    }

    Entity World::find(const std::string &name)
    {
        for (EntityMeta &entity : entities)
        {
            if (entity.name == name)
            {
                return Entity(*this, entity.id, &entity);
            }
        }

        return Entity(*this, INVALID_ENTITY, nullptr);
    }

    Entity World::find(EntityId id)
    {
        EntityMeta &meta = entities[id];
        return Entity(*this, id, &meta);
    }

    void World::destroy_entity(EntityId id)
    {
        // Create action to destroy entity
        actions.push_back(ActionInfo{
            .type = ActionInfo::Type::DestroyEntity, // Action type is to destroy entity
            .meta = {
                .id = id, // EntityId to be destroyed
            },
        });
    }

    void World::add_component(EntityId entity_id, ComponentId id, void *component)
    {
        // Find action of creating entity with the given id
        auto it = std::find_if(actions.begin(), actions.end(), [entity_id](const ActionInfo &action)
                               { return action.type == ActionInfo::Type::CreateEntity && action.meta.id == entity_id; });

        // If action is found, add component to it
        if (it != actions.end())
        {
            it->components.push_back({id, component});
            it->archetype_id.add(id);
            return;
        }

        // else create a new action to add component to entity
        actions.push_back(ActionInfo{
            .type = ActionInfo::Type::AddComponent, // Action type is to add component
            .meta = {
                .id = entity_id, // EntityId to add component to
            },
            .components = {
                {id, component}, // ComponentId and component data
            },
        });
    }

    void World::remove_component(EntityId entity_id, ComponentId id)
    {
        // Find action of creating entity with the given id
        auto it = std::find_if(actions.begin(), actions.end(), [entity_id](const ActionInfo &action)
                               { return action.type == ActionInfo::Type::CreateEntity && action.meta.id == entity_id; });

        // If action is found, remove component from it
        if (it != actions.end())
        {
            auto comp_it = std::find_if(it->components.begin(), it->components.end(), [id](const std::pair<ComponentId, void *> &component)
                                        { return component.first == id; });

            if (comp_it != it->components.end())
            {
                it->components.erase(comp_it);
            }

            return;
        }

        // else find delete component action
        it = std::find_if(actions.begin(), actions.end(), [entity_id](const ActionInfo &action)
                          { return action.type == ActionInfo::Type::RemoveComponent && action.meta.id == entity_id; });

        // If delete component action is found, add component to it
        if (it != actions.end())
        {
            it->components.push_back({id, nullptr});
            return;
        }

        // else create a new action to remove component from entity
        actions.push_back(ActionInfo{
            .type = ActionInfo::Type::RemoveComponent, // Action type is to remove component
            .meta = {
                .id = entity_id, // EntityId to remove component from
            },
            .components = {
                {id, nullptr}, // ComponentId and nullptr
            },
        });
    }

    void World::dispatch()
    {
        // Process all actions
        for (ActionInfo &action : actions)
        {
            switch (action.type)
            {
            case ActionInfo::Type::CreateEntity:
            {
                Archetype *archetype = &get_archetype(action.archetype_id);

                // Add entity to archetype
                archetype->add_entity(action.meta.id);

                // Find entity with the given id
                auto it = std::find_if(entities.begin(), entities.end(), [action](const EntityMeta &entity)
                                       { return entity.id == action.meta.id; });

                // If entity is not found, create a new one
                if (it == entities.end())
                {
                    entities.push_back({
                        .id = action.meta.id,
                        .archetype = archetype,
                        .name = action.meta.name,
                    });
                }
                else
                {
                    it->archetype = archetype;
                    it->name = action.meta.name;
                }

                // Set component data
                for (const std::pair<ComponentId, void *> &component : action.components)
                {
                    archetype->set_component(action.meta.id, component.first, component.second);
                }

                // Emit event for all component added
                ArchetypeId archetype_id({});

                for (const std::pair<ComponentId, void *> &component : action.components)
                {
                    archetype_id.add(component.first);

                    auto it = event_handlers.find(archetype_id);

                    if (it != event_handlers.end())
                    {
                        it->second->emit<Set>();
                    }
                }

                break;
            }

            case ActionInfo::Type::DestroyEntity:
            {
                // Check if entity was destroyed
                auto it = std::find(free_entities.begin(), free_entities.end(), action.meta.id);

                if (it != free_entities.end())
                {
                    break;
                }

                free_entities.push_back(action.meta.id);

                EntityMeta &meta = entities[action.meta.id];

                auto index = meta.archetype->get_entity_index(action.meta.id);

                // Delete memory of all components
                for (auto comp_id : meta.archetype->id)
                {
                    void *data = meta.archetype->component_data[comp_id][index];
                    delete_component(comp_id, data);
                    meta.archetype->component_data[comp_id][index] = nullptr;
                }

                // Remove entity from archetype
                meta.archetype->remove_entity(action.meta.id);
                meta.name.clear();
                meta.archetype = nullptr;
                break;
            }

            case ActionInfo::Type::AddComponent:
            {
                // Check if entity was destroyed
                auto it = std::find(free_entities.begin(), free_entities.end(), action.meta.id);

                if (it != free_entities.end())
                {
                    break;
                }

                // Find entity with the given id
                EntityMeta *meta = &entities[action.meta.id];

                // Get the new archetype id
                ArchetypeId old_archetype_id = meta->archetype->id.copy();
                ArchetypeId new_archetype_id = old_archetype_id.copy();

                for (const std::pair<ComponentId, void *> &component : action.components)
                {
                    new_archetype_id.add(component.first);
                }

                // Find or create new archetype
                Archetype &new_archetype = get_archetype(new_archetype_id);

                // Move entity to new archetype
                meta->archetype->move_entity(new_archetype, action.meta.id);
                meta->archetype = &new_archetype;

                // Set component data
                for (const std::pair<ComponentId, void *> &component : action.components)
                {
                    new_archetype.set_component(action.meta.id, component.first, component.second);
                }

                // Emit event for all component added
                ArchetypeId archetype_id = old_archetype_id.copy();

                for (const std::pair<ComponentId, void *> &component : action.components)
                {
                    archetype_id.add(component.first);

                    auto it = event_handlers.find(archetype_id);

                    if (it != event_handlers.end())
                    {
                        it->second->emit<Set>();
                    }
                }

                break;
            }

            case ActionInfo::Type::RemoveComponent:
            {
                // Check if entity was destroyed
                auto it = std::find(free_entities.begin(), free_entities.end(), action.meta.id);

                if (it != free_entities.end())
                {
                    break;
                }

                // Find entity with the given id
                EntityMeta *meta = &entities[action.meta.id];

                // Get the new archetype id
                ArchetypeId new_archetype_id = meta->archetype->id.copy();

                for (const std::pair<ComponentId, void *> &component : action.components)
                {
                    new_archetype_id.remove(component.first);
                }

                // Find or create new archetype
                Archetype &new_archetype = get_archetype(new_archetype_id);

                // Delete memory of removed component
                for (const std::pair<ComponentId, void *> &component : action.components)
                {
                    void *data = meta->archetype->get_component(action.meta.id, component.first);
                    delete_component(component.first, data);
                    meta->archetype->set_component(action.meta.id, component.first, nullptr);
                }

                // Move entity to new archetype
                meta->archetype->move_entity(new_archetype, action.meta.id);
                meta->archetype = &new_archetype;
                break;
            }
            }
        }

        // Clear all actions
        actions.clear();
    }

    void *World::get_component(EntityId entity_id, ComponentId id)
    {
        // Find entity with the given id
        auto it = std::find_if(entities.begin(), entities.end(), [entity_id](const EntityMeta &entity)
                               { return entity.id == entity_id; });

        assert(it != entities.end() && "Entity not found");

        // Get component from entity
        return it->archetype->get_component(entity_id, id);
    }

    Archetype &World::get_archetype(const ArchetypeId &id)
    {
        // Find archetype with the given id
        for (std::unique_ptr<Archetype> &archetype : archetypes)
        {
            if (archetype->id == id)
            {
                return *archetype.get();
            }
        }

        // If no archetype is found, create a new one
        archetypes.push_back(std::make_unique<Archetype>(*this, id));
        return *archetypes.back().get();
    }

    void World::delete_component(ComponentId id, void *component)
    {
        // Find component deleter function
        auto it = component_deleters.find(id);

        assert(it != component_deleters.end() && "Component deleter not found");

        // Call component deleter function
        it->second(component);
    }

    void World::add_event(const ArchetypeId &archetype_id, const std::type_index &event_type, const std::function<void(void)> &handler)
    {
        auto it = event_handlers.find(archetype_id);

        if (it == event_handlers.end())
        {
            event_handlers[archetype_id] = std::make_unique<EventHandler>();
        }

        event_handlers[archetype_id]->add(event_type, handler);
    }
} // namespace ecs