#pragma once

#include <functional>
#include <vector>
#include <string>
#include <memory>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

#include "windowbase.hpp"
#include "inputwindow.hpp"
#include "types.hpp"


namespace tuim {

using namespace ftxui;


class MainWindow : public WindowBase
{
private:
    int n = 0;
    int _selected_command = 0;
    int _selected_unit    = 0;
    std::vector<std::string> _commands;
    std::vector<std::string> _units;
    shared_str _unit, _info;
    ScreenInteractive _screen = ScreenInteractive::TerminalOutput();

public:
    std::function<void()> on_enter_commands;
    std::function<void()> on_enter_units;

public:
    MainWindow(StringLike auto&& title) : WindowBase(title)
    {
        _unit = Make<std::string>();
        _info = Make<std::string>();
    }

public:
    void
    render()
    {
        auto mw_renderer = renderer();

        auto input_window = InputWindow(
            "input",
            "type something"
        );
        input_window.on_ok = [&] {
            *_title = input_window.content();
            n = 0;
        };
        input_window.on_cancel = [&] { n = 0; };

        auto iw_renderer = input_window.renderer();

        auto main_container = Container::Tab({
            mw_renderer,
            iw_renderer,
        }, &n);

        auto main_renderer = Renderer(main_container, [&] {
            Element document = mw_renderer->Render();

            if (n > 0) {
                document = dbox({
                    document,
                    iw_renderer->Render() | clear_under | center,
                });
            }

            return document;
        });

        _screen.Loop(main_renderer);
    }

    Component
    renderer() override
    {
        _commands = {
            "one",
            "two",
            "three",
        };
        _units = {
            "five",
            "six",
            "seven",
        };

        auto opt1     = MenuOption();
        opt1.on_enter = on_enter_commands;

        auto opt2     = MenuOption();
        opt2.on_enter = on_enter_units;

        auto commands = Menu(&_commands, &_selected_command, opt1);
        auto units    = Menu(&_units, &_selected_unit, opt2);

        auto container = Container::Horizontal({commands, units});

        auto renderer = Renderer(container,
            [this, commands, units] {
                return window(text(*_title) | center,
                    vbox({
                        hbox({
                            text(*_unit),
                            separator(),
                            text(*_info) | flex,
                        }),
                        separator(),
                        hbox({
                            commands->Render() | flex,
                            separator(),
                            units->Render() | flex,
                        }),
                    })
                );
            }
        );

        return renderer;
    }

    void
    set_info(StringLike auto&& text)
    { *_info = std::forward<decltype(text)>(text); }

    void
    set_unit(StringLike auto&& text)
    { *_unit = std::forward<decltype(text)>(text); }

    const std::string&
    selected_command() const
    { return _commands[_selected_command]; }

    const std::string&
    selected_unit() const
    { return _units[_selected_unit]; }
};


}

