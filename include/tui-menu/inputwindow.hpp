#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <memory>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

#include "windowbase.hpp"
#include "types.hpp"


namespace tuim {

using namespace ftxui;


class InputWindow : public WindowBase
{
private: // fields
    bool _correct = true;
    std::string _info;
    std::string _content;
    std::string _placeholder;
    Component _input_field;

public: // handlers
    Validator validator;
    std::function<void()> on_ok;
    std::function<void()> on_cancel;

public: // ctors
    InputWindow(
        string_like auto&& title,
        string_like auto&& placeholder = ""
    )
        : WindowBase(title)
        , _placeholder(std::forward<decltype(placeholder)>(placeholder))
    { validator = [] (std::string_view) { return true; }; }

public: // methods
    bool
    correct() const
    { return _correct && !_content.empty(); }

    const std::string&
    content() const
    { return _content; }

    const std::string&
    get_placeholder() const
    { return _placeholder; }

    void
    set_placeholder(string_like auto&& text)
    { _placeholder = std::forward<decltype(text)>(text); }

    void
    clear_content()
    { _content.clear(); }

    void
    clear_placeholder()
    { _placeholder.clear(); }

    void
    reset()
    {
        clear_content();
        _input_field->TakeFocus();
    }

private: // methods
    std::function<void()> check_content = [&] {
        _correct = validator(_content);
    };

    Component
    renderer() override
    {
        std::function<void()> ok_handler = [this] {
            if (_correct)
                on_ok();
        };

        auto input_opt      = InputOption();
        input_opt.on_enter  = ok_handler;
        input_opt.on_change = check_content;

        _input_field            = Input(
            &_content,
            &_placeholder,
            std::move(input_opt)
        );
        Component button_ok     = Button("OK", ok_handler);
        Component button_cancel = Button("Отмена", on_cancel);

        Component container = Container::Vertical({
            _input_field,
            Container::Horizontal({
                button_ok,
                button_cancel,
            }),
        });
        Component renderer  = Renderer(container,
            [this, button_ok, button_cancel] {
                Element input_element = _input_field->Render() | border;
                if (!correct())
                    input_element = input_element | color(Color::Red);

                return window(text(_title) | center,
                    vbox({
                        text(_info),
                        input_element,
                        hbox({
                            button_ok->Render() | flex,
                            button_cancel->Render() | flex,
                        }),
                    })
                );
            }
        );

        return renderer;
    }
};


}

