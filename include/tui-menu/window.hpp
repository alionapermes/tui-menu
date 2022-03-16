#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "types.hpp"
#include "iscreen.hpp"


namespace tuim {

using namespace ftxui;


class Window
{
private:
    std::string _title;
    IScreen* _current_screen;
    ScreenInteractive _term_screen = ScreenInteractive::TerminalOutput();

public:
    Window(const std::string& title) : _title(title) {}

    Window(const std::string& title, IScreen* screen)
        : _title(title), _current_screen(screen) {}

    void
    render()
    {
        Component renderer = Renderer(_current_screen->container(), [&] {
            return window(text(_title), _current_screen->document());
        });

        _term_screen.Loop(renderer);
    }

    template <StringLike Title>
    void
    setTitle(Title&& title)
    { _title = std::forward<Title>(title); }

    std::string_view
    getTitle() const
    { return _title; }

    void
    setScreen(IScreen* screen)
    { _current_screen = screen; }
};

};

