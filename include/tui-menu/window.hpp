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
    Component modal, window_renderer, main_renderer;

    int _depth                     = 0;
    int _screen_num                = 0;
    IScreen* _main_screen          = nullptr;
    ScreenInteractive _term_screen = ScreenInteractive::Fullscreen();

public:
    Window(const std::string& title, MainScreen* main_screen)
        : _title(title), _main_screen(main_screen) {}

    void
    render()
    {
        main_renderer = Renderer(_main_screen->container(), [&] {
            return window(text(_title), _main_screen->document());
        });

        Component window_container = Container::Tab(
            {main_renderer},
            &_depth
        );

        window_renderer = Renderer(window_container, [&] {
            return main_renderer->Render();
        });

        _term_screen.Loop(window_renderer);
    }

    template <StringLike Title>
    void
    setTitle(Title&& title)
    { _title = std::forward<Title>(title); }

    std::string_view
    getTitle() const
    { return _title; }

    void
    exit()
    { ::exit(0); }

    void
    switchScreen(int num)
    {
        _screen_num = num;
        _depth = num > 0 ? 1 : 0;

        if (_depth > 0) {
            modal = Renderer(_screens[num - 1]->container(), [&] {
                return _screens[num - 1]->document();
            });
        } else
            modal = main_renderer;

        Components container = {main_renderer};
        if (modal != main_renderer)
            container.push_back(modal);

        Component window_container = Container::Tab(container, &_depth);

        Component renderer = Renderer(window_container, [&] {
            Element document = main_renderer->Render();

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

