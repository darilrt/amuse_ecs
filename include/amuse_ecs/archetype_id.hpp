#pragma once

#include <vector>
#include <iostream>

#include "amuse_ecs/ecs.hpp"

namespace ecs
{
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

        inline size_t size() const
        {
            return ids.size();
        }

        std::vector<ComponentId>::const_iterator begin() const
        {
            return ids.begin();
        }

        std::vector<ComponentId>::const_iterator end() const
        {
            return ids.end();
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
} // namespace ecs