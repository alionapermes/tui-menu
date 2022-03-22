#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "tui-menu/windowbase.hpp"
#include "mainwindow.hpp"
#include "types.hpp"


namespace tuim {

using namespace ftxui;


class TUI
{
public:
    using lid_t = int;

private:
    lid_t _lid_current = 0;
    lid_t _lid_main    = 0;
    std::vector<WindowBase*> _layers;
    ScreenInteractive _screen = ScreenInteractive::TerminalOutput();

public:
    TUI() = default;

public:
    lid_t
    add_layer(WindowBase* layer, bool as_main = false)
    {
        _layers.push_back(layer);

        lid_t id = _layers.size() - 1;
        if (as_main)
            _lid_main = id;

        return id;
    }

    void
    set_layer(lid_t lid)
    { _lid_current = lid; }

    void
    set_main_layer(lid_t lid)
    { _lid_main = lid; }

    void
    render()
    {
        Components renderers;
        for (auto& layer : _layers)
            renderers.push_back(layer->renderer());

        /* auto main_window = MainWindow("main window"); */
        /* main_window.on_enter_commands = [&] { _lid_current = 1; }; */
        /* main_window.on_enter_units    = _screen.ExitLoopClosure(); */
        /* auto mw_renderer = main_window.renderer(); */

        /* auto input_window = InputWindow( */
        /*     "input", */
        /*     "type something" */
        /* ); */
        /* input_window.on_ok = [&] { */
        /*     main_window.set_title(input_window.content()); */
        /*     _lid_current = _lid_main; */
        /* }; */
        /* input_window.on_cancel = [&] { _lid_current = _lid_main; }; */

        /* auto iw_renderer = input_window.renderer(); */

        /* auto main_container = Container::Tab({ */
        /*     mw_renderer, */
        /*     iw_renderer, */
        /* }, &_lid_current); */

        auto main_container = Container::Tab(renderers, &_lid_current);

        auto main_renderer = Renderer(main_container, [&] {
            /* Element document = mw_renderer->Render(); */
            Element document = renderers[_lid_main]->Render();

            if (_lid_current != _lid_main) {
                document = dbox({
                    document,
                    /* iw_renderer->Render() | clear_under | center, */
                    renderers[_lid_current]->Render() | clear_under | center,
                });
            }

            return document;
        });

        _screen.Loop(main_renderer);
    }
};


}

