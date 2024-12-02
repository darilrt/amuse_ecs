#pragma once

#include <cstdint>
#include "types.hpp"

typedef uint64_t EcsId;

struct World
{
    // Layout
    // |        | Entity 0 | Entity 1 | Entity 2 | ... |
    // | Comp A | Comp*    | null     | Comp*    | ... |
    // | Comp B | null     | Comp*    | null     | ... |
    // | Comp C | Comp*    | null     | null     | ... |
    // | ...    | ...      | ...      | ...      | ... |
    Vector<Vector<Ptr<void>>> components;
};