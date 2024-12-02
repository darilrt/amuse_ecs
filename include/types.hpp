#pragma once

#include <cstdint>

#include <functional>

template <typename T>
using Function = std::function<T>;

#include <memory>

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T>
using Weak = std::weak_ptr<T>;

template <typename T, typename... Args>
inline Shared<T> new_shared(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
using Unique = std::unique_ptr<T>;

template <typename T, typename... Args>
inline Unique<T> new_unique(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ptr = T *;

template <typename T>
using Ref = T &;

#include <optional>

template <typename T>
using Optional = std::optional<T>;

#include <vector>

template <typename T>
using Vector = std::vector<T>;

#include <array>

template <typename T, std::size_t N>
using Array = std::array<T, N>;

#include <map>

template <typename K, typename V>
using Map = std::map<K, V>;

#include <unordered_map>

template <typename K, typename V>
using UMap = std::unordered_map<K, V>;

#include <set>

template <typename T>
using Set = std::set<T>;

#include <unordered_set>

template <typename T>
using USet = std::unordered_set<T>;

#include <bitset>

template <std::size_t N>
using Bitset = std::bitset<N>;

#include <tuple>

template <typename... Ts>
using Tuple = std::tuple<Ts...>;

#include <variant>

template <typename... Ts>
using Variant = std::variant<Ts...>;

#include <string>

using String = std::string;

#include <filesystem>

using Path = std::filesystem::path;

#include <tuple>

template <typename... Ts>
using Tuple = std::tuple<Ts...>;

#include <utility>

template <typename T, typename U>
using Pair = std::pair<T, U>;
