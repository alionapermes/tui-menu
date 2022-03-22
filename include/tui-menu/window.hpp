#pragma once

#include <string>

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"


namespace tuim {

using namespace ftxui;


class Window
{
protected:
    std::string _title;
    Element _content;

public:
    Window() = default;

    Element
    Render()
    {
        return window(text(_title) | center, _content);
    }
};


}

