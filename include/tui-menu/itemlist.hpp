#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

#include "types.hpp"
#include "combinedcomponent.hpp"


namespace tuim {

using namespace ftxui;

class MainScreen;


template <typename Screen>
class ItemList : public CombinedComponent
{
private:
    using MenuItemList = std::vector<std::string>;
    using HandlerList  = std::vector<OnEnterFunctor<Screen>>;

    int _selected  = 0;
    MenuItemList _items;
    HandlerList _handlers;
    Component _component;
    MenuOption _option;

public:
    template <StringLike Name>
    using ItemsAddList = std::vector<std::pair<Name, OnEnterFunctor<Screen>>>;

public:
    ItemList()
    {
        _option.style_selected         = color(Color::Blue);
        _option.style_focused          = color(Color::Blue) | bold;
        _option.style_selected_focused = color(Color::Blue) | bold;
        _option.on_enter               = _on_enter;

        _component = ftxui::Menu(&_items, &_selected, &_option);
    }

    template<StringLike Name = std::string>
    void
    addItem(Name&& item, OnEnterFunctor<Screen> handler)
    {
        _items.push_back(std::forward<Name>(item));
        _handlers.push_back(handler);
    }

    template <StringLike Name = std::string>
    void
    addItems(ItemsAddList<Name>&& items)
    {
        for (auto&& [item, handler] : items)
            addItem(std::move(item), handler);
    }

    void
    removeItem(int num)
    { // todo!!!!!
        auto iter = std::find(_items.begin(), _items.end(), _items[num]);
        _items.erase(iter);
    }

    std::string
    selected() const
    { return _items[_selected]; }

private:
    std::function<void()> _on_enter = [&] {
        auto handler = _handlers[_selected];
        handler(dynamic_cast<Screen*>(_owner));
    };

    Component
    component() override
    { return _component; }
};

};

