#pragma once

#include <concepts>
#include <functional>
#include <string_view>


namespace tuim {

template <typename T>
using OnEnterFunctor = std::function<void(T*)>;

template <typename T>
concept StringLike = std::is_convertible_v<T, std::string_view>;

};

