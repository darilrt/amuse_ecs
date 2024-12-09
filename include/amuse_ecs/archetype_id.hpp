#pragma once

#include <vector>
#include <iostream>

#include "amuse_ecs/ecs.hpp"

struct ArchetypeId
{
    ArchetypeId() = default;

    ArchetypeId(const std::vector<ComponentId> &ids)
        : ids(ids)
    {
    }

    std::vector<ComponentId> ids;

    ArchetypeId copy() const;

    void add(ComponentId id);

    void remove(ComponentId id);

    bool contains(ComponentId id) const;

    bool contains(const ArchetypeId &other) const;

    template <typename... Components>
    bool contains() const
    {
        return contains(ArchetypeId({ECS_ID(Components)...}));
    }

    inline ComponentId at(size_t index) const
    {
        return ids.at(index);
    }

    bool operator==(const ArchetypeId &other) const
    {
        return ids == other.ids;
    }

    bool operator!=(const ArchetypeId &other) const
    {
        return ids != other.ids;
    }

    friend std::ostream &operator<<(std::ostream &os, const ArchetypeId &id);
};
