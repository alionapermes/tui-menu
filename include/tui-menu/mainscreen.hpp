#pragma once

#include <string>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

#include "iscreen.hpp"
#include "itemlist.hpp"
#include "combinedcomponent.hpp"


namespace tuim {

using namespace ftxui;


class MainScreen : public IScreen
{
public:
    using Menu = ItemList<MainScreen>;

private:
    std::string _unit_name;
    std::string _info_bar;
    CombinedComponent* _commands_menu = nullptr;
    CombinedComponent* _units_menu    = nullptr;

public:
    MainScreen(Menu* commands_ptr, Menu* units_ptr) 
        : _commands_menu(commands_ptr)
        , _units_menu(units_ptr)
    {
        _commands_menu->_owner = this;
        _units_menu->_owner    = this;
    }

    template <StringLike Info>
    void
    setInfoBar(Info&& info)
    { _info_bar = std::forward<Info>(info); }

    std::string_view
    getInfoBar() const
    { return _info_bar; }

    template <StringLike UnitName>
    void
    setUnitName(UnitName&& unit_name)
    { _unit_name = std::forward<UnitName>(unit_name); }

    std::string_view
    getUnitName() const
    { return _unit_name; }

private:
    Element
    document() override
    {
        return vbox({
            hbox({
                text(_unit_name),
                separator(),
                text(_info_bar) | flex,
            }),
            separator(),
            hbox({
                _commands_menu->component()->Render() | flex,
                separator(),
                _units_menu->component()->Render() | flex,
                separator(),
            }),
        });
    }

    Component
    container() override
    {
        return Container::Vertical({
            Container::Horizontal({
                _commands_menu->component(),
                _units_menu->component(),
            }),
        });
    }
};

};

