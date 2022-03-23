#pragma once

#include <concepts>
#include <string>
#include <string_view>


namespace tuim {


template <typename T>
concept string_like = std::is_convertible_v<T, std::string_view>;

template <typename T>
concept is_string = std::same_as<T, std::string>;


}

