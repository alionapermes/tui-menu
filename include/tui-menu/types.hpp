#pragma once

#include <concepts>
#include <functional>
#include <string_view>
#include <type_traits>

#include "iscreen.hpp"


namespace tuim {

/* class IScreen; */

/* template <typename Child> */
/* concept IScreenBased = std::is_base_of_v<IScreen, Child>; //std::derived_from<T, IScreen>; */

/* class MainScreen; */
/* template <IScreenBased Screen> */
template <typename T>
using OnEnterFunctor         = std::function<void(T*)>;
/* using OnEnterFunctor2        = std::function<void(MainScreen*)>; */

/* template <IScreenBased Screen> */
template <typename T>
using OnEnterFunctionPointer  = void (*)(T*);
/* using OnEnterFunctionPointer2 = void (*)(MainScreen*); */
/* typedef void (*OnEnterFunctionPointer)(IScreen*); */

template <typename T>
concept StringLike = std::is_convertible_v<T, std::string_view>;

template <typename Handler, typename Screen>
concept OnEnterHandler = (
    std::same_as<Handler, OnEnterFunctor<Screen>>//<ArgType>>
    || std::same_as<Handler, OnEnterFunctionPointer<Screen>>//<ArgType>>
    /* || std::same_as<Handler, OnEnterFunctor2>//<ArgType>> */
    /* || std::same_as<Handler, OnEnterFunctionPointer2>//<ArgType>> */
);

};

