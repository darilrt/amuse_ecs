#include <algorithm>

#include "amuse_ecs/archetype_id.hpp"

ArchetypeId ArchetypeId::copy() const
{
    ArchetypeId copy;
    copy.ids = ids;
    return copy;
}

void ArchetypeId::add(ComponentId id)
{
    if (std::find(ids.begin(), ids.end(), id) == ids.end())
    {
        ids.push_back(id);
        std::sort(ids.begin(), ids.end());
    }
}

void ArchetypeId::remove(ComponentId id)
{
    auto it = std::find(ids.begin(), ids.end(), id);

    if (it != ids.end())
    {
        ids.erase(it);
    }

    std::sort(ids.begin(), ids.end());
}

bool ArchetypeId::contains(ComponentId id) const
{
    return std::find(ids.begin(), ids.end(), id) != ids.end();
}

bool ArchetypeId::contains(const ArchetypeId &other) const
{
    for (auto id : other.ids)
    {
        if (!contains(id))
        {
            return false;
        }
    }

    return true;
}

std::ostream &operator<<(std::ostream &os, const ArchetypeId &id)
{
    os << "ArchetypeId[";

    for (size_t i = 0; i < id.ids.size(); i++)
    {
        os << id.ids[i];

        if (i < id.ids.size() - 1)
        {
            os << ", ";
        }
    }

    os << "]";

    return os;
}