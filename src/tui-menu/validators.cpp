#include "tui-menu/validators.hpp"


bool
tuim::
is_numeric(std::string_view buffer)
{
    bool dot_once = false;
    float num     = 0;
    std::string str(buffer);

    for (const auto& ltr : str) {
        if (!dot_once && ltr == '.')
            dot_once = true;
        else if (ltr < '0' || ltr > '9')
            return false;
    }

    try {
        num = std::stof(str);
        return true;
    } catch (std::invalid_argument&) {
        return false;
    }
};

