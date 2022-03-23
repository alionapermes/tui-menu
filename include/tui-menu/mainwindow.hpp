#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

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


template <typename T>
class MainWindow : public WindowBase
{
private: // data types
    using ItemsList = std::vector<
        std::pair<std::string, std::function<void()>>
    >;

    class ItemsMenu
    {
    private:
        int _selected = 0;
        std::vector<std::string> _items;
        std::vector<std::function<void()>> _handlers;

    public:
        std::vector<std::string>&
        items()
        { return _items; }

        int&
        selected()
        { return _selected; }

        const std::string&
        selected_name() const
        { return _items[_selected]; }

        void
        exec()
        { _handlers[_selected](); }

        std::vector<std::string>::iterator
        find(const std::string& item)
        { return std::find(_items.begin(), _items.end(), item); }

        bool
        erase(const std::string& item)
        {
            auto iter = find(item);
            if (iter == _items.end())
                return false;

            _items.erase(iter);
            _selected = (_selected > 0 ? _selected - 1 : 0);

            return true;
        }

        void
        add(string_like auto&& item, std::function<void()> handler)
        {
            _items.push_back(std::forward<decltype(item)>(item));
            _handlers.push_back(handler);
        }

        bool
        empty() const
        { return _items.empty(); }

        size_t
        size() const
        { return _items.size(); }
    };

private: // fields
    std::string _unit_name, _info;
    std::unordered_map<std::string, T> _containers;
    ItemsMenu _commands, _units;
    ScreenInteractive _screen = ScreenInteractive::TerminalOutput();

public: // ctors
    MainWindow(string_like auto&& title) : WindowBase(title) {}

public: // methods
    const std::string&
    get_info() const
    { return _info; }

    void
    set_info(string_like auto&& text)
    { _info = std::forward<decltype(text)>(text); }

    const std::string&
    selected_command() const
    { return _commands.selected_name(); }

    const std::string&
    selected_unit_name() const
    { return _units.selected_name(); }

    void
    add_commands(ItemsList&& commands)
    {
        for (const auto& [name, handler] : commands)
            _commands.add(name, handler);
    }

    template <typename... Args>
    bool
    add_unit(string_like auto&& unit, Args... args)
    {
        if (_containers.contains(unit))
            return false;

        _containers[unit] = T(args...);
        _units.add(std::forward<decltype(unit)>(unit), [this] {
            _unit_name = selected_unit_name();
        });

        return true;
    }

    bool
    remove_unit(const std::string& name)
    { return _units.erase(name); }

    T&
    current_unit()
    {
        if (current_unit_name().empty())
            return _containers[selected_unit_name()];
        else
            return _containers[current_unit_name()];
    }

    const std::string&
    current_unit_name() const
    { return _unit_name; }

    bool
    units_empty() const
    { return _units.empty(); }

    size_t
    units_count() const
    { return _units.size(); }

private: // methods
    Component
    renderer() override
    {
        auto commands_opt     = MenuOption();
        commands_opt.on_enter = [this] { _commands.exec(); };

        auto units_opt     = MenuOption();
        units_opt.on_enter = [this] { _units.exec(); };

        Component commands = Menu(
            &_commands.items(),
            &_commands.selected(),
            std::move(commands_opt)
        );
        Component units    = Menu(
            &_units.items(),
            &_units.selected(),
            std::move(units_opt)
        );

        Component container = Container::Horizontal({commands, units});
        Component renderer  = Renderer(container,
            [this, commands, units] {
                return window(text(_title) | center,
                    vbox({
                        hbox({
                            text(_unit_name),
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

