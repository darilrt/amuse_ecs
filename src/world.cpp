#include "amuse_ecs/world.hpp"
#include "amuse_ecs/entity.hpp"
#include "amuse_ecs/archetype.hpp"
#include "amuse_ecs/view.hpp"

World::World()
{
    entity_count = 0;
    component_count = 0;

    // Create an archetype with no components
    archetypes.push_back(std::make_unique<Archetype>(*this));
}

Entity World::create_entity(const std::string &name)
{
    EntityId id;

    // Get the archetype with no components
    Archetype &archetype = *archetypes[0];

    if (free_entities.empty())
    {
        id = entity_count++;
        entity_info.push_back({id, &archetype, name});
    }
    else
    {
        id = free_entities.back();
        free_entities.pop_back();
        entity_info[id].id = id;
        entity_info[id].archetype = &archetype;
        entity_info[id].name = name;
    }

    archetype.add_entity(id);

    return Entity(id, *this);
}

void World::destroy_entity(EntityId id)
{
    assert(find_entity(id) != INVALID_ENTITY && "Entity does not exist");

    entity_info[id].archetype->delete_entity(id);

    free_entities.push_back(id);
}

// Find an entity by id
// Returns INVALID_ENTITY if the entity does not exist (That is, the entity id is greater than the entity count or the entity id is in the free entities list)
EntityId World::find_entity(EntityId id)
{
    if (id >= entity_count)
    {
        return INVALID_ENTITY;
    }

    auto it = std::find(free_entities.begin(), free_entities.end(), id);

    if (it != free_entities.end())
    {
        return INVALID_ENTITY;
    }

    return id;
}
