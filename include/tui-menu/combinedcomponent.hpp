#pragma once

#include "ftxui/component/component_base.hpp"

#include "iscreen.hpp"


namespace tuim {

using namespace tuim;


class CombinedComponent
{
public:
    IScreen* _owner;

    virtual Component
    component() = 0;
};

};

