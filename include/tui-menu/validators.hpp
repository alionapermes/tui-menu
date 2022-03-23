#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>


namespace tuim {


std::function<bool(std::string_view)>
is_numeric();

std::function<bool(std::string_view)>
length_range(size_t minimum, size_t oversize);


}

