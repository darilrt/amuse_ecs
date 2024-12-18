#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <utility>
#include <cassert>
#include <list>

#include "amuse_ecs/ecs.hpp"
#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/entity_info.hpp"
#include "amuse_ecs/entity.hpp"
#include "amuse_ecs/view.hpp"
#include "amuse_ecs/event_handler.hpp"

namespace ecs
{
    // Set component event
    EVENT(Set);

    // Unset component event
    EVENT(Unset);

    class World
    {
        struct ActionInfo
        {
            enum class Type
            {
                CreateEntity,
                DestroyEntity,
                AddComponent,
                RemoveComponent
            } type;
            EntityMeta meta;
            ArchetypeId archetype_id;
            std::vector<std::pair<ComponentId, void *>> components;
        };

    public:
        std::unordered_map<ArchetypeId, std::unique_ptr<EventHandler>> event_handlers;
        std::vector<EntityMeta> entities;
        std::vector<std::unique_ptr<Archetype>> archetypes;

        EntityId next_entity_id = 0;
        std::vector<EntityId> free_entities;

        // Actions to be processed at the end of the frame or at the end of the world update
        // This is used to avoid modifying the world while iterating over it
        std::list<ActionInfo> actions;

        // Map of component delete functions
        std::unordered_map<ComponentId, std::function<void(void *)>> component_deleters;

        World();

        // Register a new action to create a new entity
        // This action will be processed when dispatch is called
        Entity entity(const std::string &name = "");

        Entity entity_with_components(const std::string &name, const std::vector<std::pair<ComponentId, void *>> &components);

        template <typename... Components>
        Entity entity(const std::string &name = "", Components... components)
        {
            return entity_with_components(name, {std::make_pair(ECS_ID(Components), static_cast<void *>(&components))...});
        }

        Entity find(const std::string &name);

        Entity find(EntityId id);

        void destroy_entity(EntityId id);

        void add_component(EntityId entity_id, ComponentId id, void *component);

        void remove_component(EntityId entity_id, ComponentId id);

        void dispatch();

        void *get_component(EntityId entity_id, ComponentId id);

        template <typename... Components>
        ArchetypeId get_archetype_id()
        {
            ArchetypeId id;

            (id.add(ECS_ID(Components)), ...);

            return id;
        }

        Archetype &get_archetype(const ArchetypeId &id);

        template <typename... Components>
        Archetype &get_archetype()
        {
            ArchetypeId id = get_archetype_id<Components...>();
            return get_archetype(id);
        }

        void delete_component(ComponentId id, void *components);

        template <typename Component>
        void delete_component(void *component)
        {
            delete_component(ECS_ID(Component), component);
        }

        template <typename Component>
        void register_component()
        {
            component_deleters[ECS_ID(Component)] = [](void *component)
            {
                delete static_cast<Component *>(component);
            };
        }

        template <typename... Component>
        View<Component...> view()
        {
            View<Component...> view(*this);

            for (auto &archetype : archetypes)
            {
                if (archetype->id.contains<Component...>())
                {
                    view.archetypes.push_back(archetype.get());
                }
            }

            return view;
        }

        template <typename Event>
        inline void emit(const ArchetypeId &archetype_id)
        {
            auto it = event_handlers.find(archetype_id);
            if (it != event_handlers.end())
            {
                it->second->emit<Event>();
            }
        }

        void add_event(const ArchetypeId &archetype_id, const std::type_index &event_type, const std::function<void(void)> &handler);

        template <typename Event>
        inline void on(const ArchetypeId &archetype_id, const std::function<void(void)> &handler)
        {
            add_event(archetype_id, std::type_index(typeid(Event)), handler);
        }
    };
} // namespace ecs