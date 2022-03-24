#include "tui-menu/validators.hpp"


tuim::Validator
operator|(const tuim::Validator& lhs, const tuim::Validator& rhs)
{
    return [&lhs, &rhs](std::string_view buffer) -> bool {
        return lhs(buffer) && rhs(buffer);
    };
}

tuim::Validator
tuim::is_numeric()
{
    return [](std::string_view buffer) -> bool {
        bool dot_once  = false;
        float num      = 0;
        size_t n       = (buffer.front() == '-' ? 1 : 0);

        for (; n < buffer.length(); n++) {
            if (!dot_once && buffer[n] == '.')
                dot_once = true;
            else if (buffer[n] < '0' || buffer[n] > '9')
                return false;
        }

        try {
            num = std::stof(std::string(buffer));
            return true;
        } catch (const std::invalid_argument&) {
            return false;
        }
    };
};

tuim::Validator
tuim::length_range(size_t minimum, size_t maximum)
{
    return [minimum, maximum](std::string_view buffer) -> bool {
        return ((minimum <= buffer.length()) && (buffer.length() <= maximum));
    };
}

tuim::Validator
tuim::in_range(float minimum, float maximum)
{
    return [minimum, maximum](std::string_view buffer) -> bool {
        float num = std::stof(std::string(buffer));
        return ((minimum <= num) && (num <= maximum));
    };
}

