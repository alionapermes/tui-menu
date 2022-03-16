#pragma once

/* #include <concepts> */

#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/node.hpp"


namespace tuim {

using namespace ftxui;

class Window;


class IScreen
{
public:
    virtual Component
    container() = 0;

    virtual Element
    document() = 0;
};

/* template <typename Child> */
/* concept IScreenBased = std::is_base_of_v<IScreen, Child>; */

};

