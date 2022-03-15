#include <concepts>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"


class MainScreen;

typedef std::function<void(MainScreen*)> OnEnterFunctor;
typedef void (*OnEnterFunctionPointer)(MainScreen*);

template <typename T>
concept StringLike = std::is_convertible_v<T, std::string_view>;

template <typename T>
concept OnEnterHandler = (
    std::same_as<T, OnEnterFunctor>
    || std::same_as<T, OnEnterFunctionPointer>
);


class MainScreen
{
public:   
    class Menu
    {
        friend MainScreen;

    public:
        using MenuItemList   = std::vector<std::string>;
        using HandlerList    = std::vector<OnEnterFunctor>;

        template <StringLike Name, OnEnterHandler Handler>
        using ItemsAddList   = std::vector<std::pair<Name, Handler>>;


        Menu()
        {
            using namespace ftxui;

            _option.style_selected         = color(Color::Blue);
            _option.style_focused          = color(Color::Blue) | bold;
            _option.style_selected_focused = color(Color::Blue) | bold;
            _option.on_enter               = _on_enter;

            _component = ftxui::Menu(&_items, &_selected, &_option);
        }

        template<StringLike Name>
        void
        addItem(Name&& item, OnEnterFunctor handler)
        {
            _items.push_back(std::forward<Name>(item));
            _handlers.push_back(handler);
        }

        template<StringLike Name>
        void
        addItem(Name&& item, void (*handler)(MainScreen*))
        {
            _items.push_back(std::forward<Name>(item));
            _handlers.push_back([=](MainScreen* screen) {
                handler(screen);
            });
        }

        template <
            OnEnterHandler Handler = OnEnterFunctionPointer,
            StringLike Name        = std::string
        > void
        addItems(ItemsAddList<Name, Handler>&& items)
        {
            for (auto&& [item, handler] : items)
                addItem(std::move(item), handler);
        }

    private:
        int _selected      = 0;
        MainScreen* _owner = nullptr;
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

public:
    std::string Title;
    std::string InfoBar;
    std::string UnitName;
    std::string InputField;

private:
    ftxui::ScreenInteractive _screen =
        ftxui::ScreenInteractive::TerminalOutput();
    Menu* _commands_menu;
    Menu* _units_menu;

public:
    MainScreen(const std::string& title, Menu* commands_ptr, Menu* units_ptr)
        : Title(title)
        , _commands_menu(commands_ptr)
        , _units_menu(units_ptr)
    {
        _commands_menu->_owner = this;
        _units_menu->_owner    = this;
    }

    void
    render()
    {
        using namespace ftxui;

        Component input_field = Input(&InputField, "placeholder");
        Component container   = Container::Horizontal({
            _commands_menu->component(),
            _units_menu->component(),
        });

        Component renderer = Renderer(container, [&] {
            return
                vbox({
                    hcenter(text(Title)) | bold,
                    separator(),
                    hbox({
                        text(UnitName),
                        separator(),
                        text(InfoBar) | flex,
                    }),
                    separator(),
                    hbox({
                        _commands_menu->component()->Render() | flex,
                        separator(),
                        _units_menu->component()->Render() | flex,
                        separator(),
                    }),
                    input_field->Render() | flex | border,
                }) | border;
        });

        _screen.Loop(renderer);
    }
};

