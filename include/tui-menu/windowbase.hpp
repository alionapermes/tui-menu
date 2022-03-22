#pragma once

#include <string>
#include <utility>

#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

#include "types.hpp"


namespace tuim {

using namespace ftxui;


class WindowBase
{
public:
    using shared_str = std::shared_ptr<std::string>;

protected:
    shared_str _title;

public: // ctors
    WindowBase(StringLike auto&& title)
        : _title(Make<std::string>(std::forward<decltype(title)>(title))) {}

public: // methods
    const std::string&
    get_title() const
    { return *_title; }

    void
    set_title(StringLike auto&& title)
    { *_title = std::forward<decltype(title)>(title); }

public: // virtual methods
    Component
    renderer() {}// = 0;
};


}

