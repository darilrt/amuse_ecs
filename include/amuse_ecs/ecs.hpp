#pragma once

#include <cstdint>
#include <typeindex>

typedef uint64_t EcsId;
typedef EcsId EntityId;
typedef std::type_index ComponentId;

#define ECS_ID(name) std::type_index(typeid(name))

enum : EcsId
{
    INVALID_ID = 0xFFFFFFFFFFFFFFFF
};

enum : EntityId
{
    INVALID_ENTITY = INVALID_ID
};