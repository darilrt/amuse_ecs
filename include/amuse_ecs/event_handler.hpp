#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>

#define EVENT(name) \
    struct name     \
    {               \
    };

namespace ecs
{
    class EventHandler
    {
        std::unordered_map<std::type_index, std::vector<std::function<void(void)>>> _handlers;

    public:
        void add(const std::type_index &type, const std::function<void(void)> &handler)
        {
            _handlers[type].push_back(handler);
        }

        template <typename Event>
        void on(const std::function<void(void)> &handler)
        {
            add(std::type_index(typeid(Event)), handler);
        }

        template <typename Event>
        void emit()
        {
            auto it = _handlers.find(std::type_index(typeid(Event)));
            if (it != _handlers.end())
            {
                for (auto &handler : it->second)
                {
                    handler();
                }
            }
        }
    };
} // namespace ecs