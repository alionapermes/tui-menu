#pragma once

#include <string>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

#include "types.hpp"
#include "iscreen.hpp"


namespace tuim {

using namespace ftxui;

class MainScreen;

template <typename Screen = MainScreen>
class Menu
{
    friend MainScreen;

public:
    using MenuItemList = std::vector<std::string>;
    using HandlerList  = std::vector<OnEnterFunctor<Screen>>;

    template <StringLike Name, OnEnterHandler<Screen> Handler>
    using ItemsAddList = std::vector<std::pair<Name, Handler>>;


    Menu()
    {
        _option.style_selected         = color(Color::Blue);
        _option.style_focused          = color(Color::Blue) | bold;
        _option.style_selected_focused = color(Color::Blue) | bold;
        _option.on_enter               = _on_enter;

        _component = ftxui::Menu(&_items, &_selected, &_option);
    }

    template<StringLike Name>
    void
    addItem(Name&& item, OnEnterFunctor<Screen> handler)
    {
        _items.push_back(std::forward<Name>(item));
        _handlers.push_back(handler);
    }

    template<StringLike Name>
    void
    addItem(Name&& item, void (*handler)(Screen*))
    {
        _items.push_back(std::forward<Name>(item));
        _handlers.push_back([=](Screen* screen) {
            handler(screen);
        });
    }

    template <
        OnEnterHandler<Screen> Handler = OnEnterFunctionPointer<Screen>,
        StringLike Name                = std::string
    > void
    addItems(ItemsAddList<Name, Handler>&& items)
    {
        for (auto&& [item, handler] : items)
            addItem(std::move(item), handler);
    }

private:
    int _selected  = 0;
    Screen* _owner = nullptr;
    MenuItemList _items;
    HandlerList _handlers;
    ftxui::Component _component;
    ftxui::MenuOption _option;


    std::function<void()> _on_enter = [&] {
        auto handler = _handlers[_selected];
        handler(_owner);
    };

    ftxui::Component
    component()
    { return _component; }
};

};

