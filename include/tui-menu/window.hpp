#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

#include "types.hpp"
#include "iscreen.hpp"
#include "mainscreen.hpp"


namespace tuim {

using namespace ftxui;


class Window
{
private:
    std::string _title;
    std::vector<IScreen*> _screens;
    Component _main_renderer;

    int _depth                     = 0;
    IScreen* _main_screen          = nullptr;
    ScreenInteractive _term_screen = ScreenInteractive::Fullscreen();

public:
    Window(const std::string& title, MainScreen* main_screen)
        : _title(title), _main_screen(main_screen) {}

    void
    render()
    {
        _main_renderer = Renderer(_main_screen->container(), [&] {
            return window(text(_title), _main_screen->document());
        });

        Component window_container = Container::Tab(
            {_main_renderer},
            &_depth
        );

        Component window_renderer = Renderer(window_container, [&] {
            return _main_renderer->Render();
        });

        _term_screen.Loop(window_renderer);
    }

    void
    setTitle(StringLike auto&& title)
    { _title = std::forward<decltype(title)>(title); }

    std::string_view
    getTitle() const
    { return _title; }

    void
    exit()
    { ::exit(0); }

    void
    switchScreen(int num)
    {
        _depth = static_cast<int>(num > 0);

        Component modal = _main_renderer;
        if (_depth > 0) {
            modal = Renderer(_screens[num - 1]->container(), [&] {
                return _screens[num - 1]->document();
            });
        }

        Components container = {_main_renderer};
        if (modal != _main_renderer)
            container.push_back(modal);

        Component window_container = Container::Tab(container, &_depth);

        Component renderer = Renderer(window_container, [&] {
            Element document = _main_renderer->Render();

            if (_depth > 0) {
                document = dbox({
                    document,
                    modal->Render() | clear_under | center
                });
            }

            return document;
        });

        _term_screen.Loop(renderer);
    }

    int
    addModal(IScreen* screen)
    {
        _screens.push_back(screen);
        return _screens.size();
    }
};

};

