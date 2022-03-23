#pragma once

#include <concepts>
#include <string_view>


namespace tuim {


template <typename T>
concept string_like = std::is_convertible_v<T, std::string_view>;


}

