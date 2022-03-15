#include <concepts>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"


template<typename T>
concept StringLike = std::is_convertible_v<T, std::string_view>;


class MainScreen
{
public:   
    class Menu
    {
        friend MainScreen;

    public:
        using OnEnterHandler = std::function<void(MainScreen*)>;
        using HandlerMap     = std::unordered_map<int, OnEnterHandler>;
        using MenuItemList   = std::vector<std::string>;
        using HandlerList    = std::vector<OnEnterHandler>;
        using ItemsAddList   = std::vector<std::pair<
            std::string,
            OnEnterHandler
        >>;

        Menu(MainScreen* owner = nullptr) : _owner(owner)
        {
            using namespace ftxui;

            _option.style_selected         = color(Color::Blue);
            _option.style_focused          = color(Color::Blue) | bold;
            _option.style_selected_focused = color(Color::Blue) | bold;
            _option.on_enter               = _on_enter;

            _component = ftxui::Menu(&_items, &_selected, &_option);
        }

        Menu(const Menu& other) = default;

        Menu(Menu&& other)
        { *this = std::move(other); }

        Menu(const Menu& other, MainScreen* screen_ptr) : _owner(screen_ptr)
        { *this = other; }

        Menu&
        operator=(const Menu& other) = default;

        Menu&
        operator=(Menu&& other) noexcept = default;

        template<StringLike Name>
        void
        addItem(Name&& item, OnEnterHandler handler)
        {
            _items.push_back(std::forward<Name>(item));
            _handlers.push_back(handler);
        }

        void
        addItems(ItemsAddList&& items)
        {
            for (auto&& [item, handler] : items)
                addItem(std::move(item), handler);
        }

        ftxui::Component
        component()
        { return _component; }

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
    MainScreen() = default;

    MainScreen(const std::string& title) : Title(title) {}

    MainScreen(const std::string& title, Menu* commands, Menu* units)
        : Title(title)
        , _commands_menu(commands)
        , _units_menu(units)
    {
        _commands_menu->_owner = this;
        _units_menu->_owner = this;
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

