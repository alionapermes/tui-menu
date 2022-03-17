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
    Components _renderers;

    int _depth                         = 0;
    IScreen* main_screen = nullptr;
    IScreen* inp_screen  = nullptr;
    /* Component main_renderer; */
    Component modal, window_renderer, main_renderer;
    ScreenInteractive _term_screen = ScreenInteractive::Fullscreen();

public:
    Window(const std::string& title, MainScreen* main_screen, IScreen* ptr = nullptr) : _title(title)
    {
        /* auto screen = dynamic_cast<IScreen*>(main_screen); */

        /* _screens.push_back(main_screen); */

        this->main_screen = main_screen;
        this->inp_screen  = ptr;
    }

    void
    render()
    {
        main_renderer = Renderer(main_screen->container(), [&] {
            return window(text(_title), main_screen->document());
        });
        /* Component inp_renderer = Renderer(inp_screen->container(), [&] { */
        /*     return inp_screen->document(); */
        /* }); */


        IScreen* modal_screen = inp_screen;
        modal = Renderer(modal_screen->container(), [&] {
            return modal_screen->document();
        });

        Component window_container = Container::Tab(
            {main_renderer, modal},
            &_depth
        );

        window_renderer = Renderer(window_container, [&] {
            Element document = main_renderer->Render();

            if (_depth > 0) {
                document = dbox({
                    document,
                    modal->Render() | clear_under | center
                });
            }

            return document;
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
    switchScreen(int depth)
    {
        _depth = depth;

        if (depth > 0) {
            modal = Renderer(_screens[depth - 1]->container(), [&] {
                return _screens[depth - 1]->document();
            });
        } else
            modal = main_renderer;

        Component window_container = Container::Tab(
            {main_renderer, modal},
            &_depth
        );

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

        /* _term_screen.Clear(); */
        /* _term_screen.ResetPosition(); */
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

