#pragma once

#include <cstdint>
#include <typeindex>

typedef uint64_t EcsId;
typedef EcsId EntityId;
typedef EcsId ComponentId;

#define ECS_ID(name) ComponentTypeIndex::index_of<name>()

class ComponentTypeIndex
{
    static ComponentId last_index;

public:
    template <typename C>
    inline static ComponentId index_of()
    {
        static ComponentId id = ComponentTypeIndex::last_index++;
        return id;
    }
};

enum : EcsId
{
    INVALID_ID = 0xFFFFFFFFFFFFFFFF
};

enum : EntityId
{
    INVALID_ENTITY = INVALID_ID
};