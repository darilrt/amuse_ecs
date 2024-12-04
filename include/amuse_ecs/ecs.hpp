#pragma once

#include <cstdint>

typedef uint64_t EcsId;
typedef EcsId EntityId;
typedef EcsId ComponentId;

enum : EcsId
{
    INVALID_ID = 0xFFFFFFFFFFFFFFFF
};

enum : EntityId
{
    INVALID_ENTITY = INVALID_ID
};

enum : ComponentId
{
    INVALID_COMPONENT = INVALID_ID
};