#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "types.hpp"


tuim::Validator
operator|(const tuim::Validator& lhs, const tuim::Validator& rhs);


namespace tuim {


Validator
is_numeric();

Validator
length_range(size_t minimum, size_t maximum);

Validator
in_range(float minimum, float maximum);


}

