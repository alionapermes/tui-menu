#pragma once

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
    shared_str _unit, _info;
    ScreenInteractive _screen = ScreenInteractive::TerminalOutput();

public:
    MainWindow(StringLike auto&& title) : WindowBase(title)
    {
        _unit = Make<std::string>();
        _info = Make<std::string>();
    }

public:
    /* [[noreturn]] */
    /* Component */
    /* renderer() override; */

    void
    render()
    {
        int n = 0, n1 = 0, n2 = 0;
        std::vector<std::string> items1 = {
            "one",
            "two",
            "three",
        };
        std::vector<std::string> items2 = {
            "five",
            "six",
            "seven",
        };

        auto opt1     = MenuOption();
        opt1.on_enter = [&] { *_unit = items1[n1]; n = 1; };

        auto opt2     = MenuOption();
        opt2.on_enter = [&] { *_info = items2[n2]; n = 1; };

        auto menu1 = Menu(&items1, &n1, &opt1);
        auto menu2 = Menu(&items2, &n2, &opt2);

        auto container = Container::Horizontal({
            menu1,
            menu2,
        });

        auto renderer1 = Renderer(container, [&] {
            return window(text(*_title) | center,
                vbox({
                    hbox({
                        text(*_unit),
                        separator(),
                        text(*_info) | flex,
                    }),
                    separator(),
                    hbox({
                        menu1->Render() | flex,
                        separator(),
                        menu2->Render() | flex,
                    }),
                })
            );
        });

        auto input_window = InputWindow(
            "input",
            "type something"
        );
        input_window.on_ok = [&] {
            *_title = input_window.content();
            n = 0;
        };
        input_window.on_cancel = [&] { n = 0; };

        auto renderer2 = input_window.renderer();

        auto main_container = Container::Tab({
            renderer1,
            renderer2,
        }, &n);

        auto main_renderer = Renderer(main_container, [&] {
            Element document = renderer1->Render();

            if (n > 0) {
                document = dbox({
                    document,
                    renderer2->Render() | clear_under | center,
                });
            }

            return document;
        });

        _screen.Loop(main_renderer);
    }

    void
    setInfo(StringLike auto&& text)
    { *_info = std::forward<decltype(text)>(text); }

    void
    setUnit(StringLike auto&& text)
    { *_unit = std::forward<decltype(text)>(text); }
};


}

