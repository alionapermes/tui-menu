#pragma once

#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/node.hpp"


namespace tuim {

using namespace ftxui;


class IScreen
{
public:
    virtual Component
    container() = 0;

    virtual Element
    document() = 0;
};

};

