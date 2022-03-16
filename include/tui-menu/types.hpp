#pragma once

#include <concepts>
#include <functional>
#include <string_view>
#include <type_traits>


namespace tuim {

template <typename T>
using OnEnterFunctor = std::function<void(T*)>;

template <typename T>
using OnEnterFunctionPointer = void (*)(T*);

template <typename T>
concept StringLike = std::is_convertible_v<T, std::string_view>;

template <typename Handler, typename Screen>
concept OnEnterHandler = (
    std::same_as<Handler, OnEnterFunctor<Screen>>
    || std::same_as<Handler, OnEnterFunctionPointer<Screen>>
);

};

