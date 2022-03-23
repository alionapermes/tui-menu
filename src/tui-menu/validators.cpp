#include "tui-menu/validators.hpp"


std::function<bool(std::string_view)>
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

std::function<bool(std::string_view)>
tuim::length_range(size_t minimum, size_t oversize)
{
    return [minimum, oversize](std::string_view buffer) -> bool {
        return minimum <= buffer.length() && buffer.length() < oversize;
    };
}

