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

#include "windowbase.hpp"
#include "types.hpp"


namespace tuim {

using namespace ftxui;


class MainWindow : public WindowBase
{
private: // data types
    template <string_like Text>
    using ItemsList = std::vector<std::pair<Text, std::function<void()>>>;

    class ItemsMenu
    {
        friend MainWindow;

    public:
        int
        selected() const
        { return _selected; }

        const std::string&
        selected_name() const
        { return _items[_selected]; }

        void
        exec()
        { _handlers[_selected](); }

    private:
        int _selected = 0;
        std::vector<std::string> _items;
        std::vector<std::function<void()>> _handlers;
    };

private: // fields
    ItemsMenu _commands, _units;
    std::string _unit, _info;
    ScreenInteractive _screen = ScreenInteractive::TerminalOutput();

public: // ctors
    MainWindow(string_like auto&& title) : WindowBase(title) {}

public: // methods
    void
    set_info(string_like auto&& text)
    { _info = std::forward<decltype(text)>(text); }

    void
    set_unit(string_like auto&& text)
    { _unit = std::forward<decltype(text)>(text); }

    const std::string&
    selected_command() const
    { return _commands.selected_name(); }

    const std::string&
    selected_unit() const
    { return _units.selected_name(); }

    template <string_like Text = std::string>
    void
    add_commands(ItemsList<Text> commands)
    {
        for (const auto& [name, handler] : commands) {
            _commands._items.push_back(name);
            _commands._handlers.push_back(handler);
        }
    }

private: // methods
    Component
    renderer() override
    {
        auto commands_opt     = MenuOption();
        commands_opt.on_enter = [this] { _commands.exec(); };

        auto units_opt     = MenuOption();
        units_opt.on_enter = [this] { _units.exec(); };

        Component commands = Menu(
            &_commands._items,
            &_commands._selected,
            std::move(commands_opt)
        );
        Component units    = Menu(
            &_units._items,
            &_units._selected,
            std::move(units_opt)
        );

        Component container = Container::Horizontal({commands, units});
        Component renderer  = Renderer(container,
            [this, commands, units] {
                return window(text(_title) | center,
                    vbox({
                        hbox({
                            text(_unit),
                            separator(),
                            text(_info) | flex,
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
};


}

