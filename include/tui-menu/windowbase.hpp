#pragma once

#include <string>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

#include "types.hpp"


namespace tuim {

using namespace ftxui;


class WindowBase
{
public: // aliases
    using shared_str = std::shared_ptr<std::string>;

protected: // fields
    shared_str _title;

public: // ctors
    WindowBase(string_like auto&& title)
        : _title(Make<std::string>(std::forward<decltype(title)>(title))) {}

public: // methods
    const std::string&
    get_title() const
    { return *_title; }

    void
    set_title(string_like auto&& title)
    { *_title = std::forward<decltype(title)>(title); }

public: // virtual methods
    virtual Component
    renderer() = 0;
};


}

