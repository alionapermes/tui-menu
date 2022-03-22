#pragma once

#include <concepts>
#include <functional>
#include <string_view>


using lid_t = int;


namespace tuim {


template <typename T>
concept string_like = std::is_convertible_v<T, std::string_view>;


}

