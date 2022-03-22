#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <memory>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

#include "windowbase.hpp"
#include "types.hpp"


namespace tuim {

using namespace ftxui;


class InputWindow : public WindowBase
{
private:
    bool _correct = true;
    shared_str _info;
    shared_str _content;
    shared_str _placeholder;

public:
    std::function<bool(std::string_view)> validator;
    std::function<void()> on_ok;
    std::function<void()> on_cancel;

public:
    InputWindow(
        StringLike auto&& title = "",
        StringLike auto&& placeholder = ""
    )
        : WindowBase(title)
        , _info(Make<std::string>())
        , _content(Make<std::string>())
        , _placeholder(Make<std::string>(
            std::forward<decltype(placeholder)>(placeholder)
        ))
    {
        validator = [] (std::string_view) { return true; };
    }

public:
    Component
    renderer() //override
    {
        auto input_opt      = InputOption();
        input_opt.on_enter  = on_ok;
        input_opt.on_change = check_content;

        auto input_field = Input(
            &*_content,
            &*_placeholder,
            std::move(input_opt)
        );

        auto button_ok     = Button("OK", on_ok);
        auto button_cancel = Button("Отмена", on_cancel);

        auto container = Container::Vertical({
            input_field,
            Container::Horizontal({
                button_ok,
                button_cancel,
            }),
        });

        auto renderer = Renderer(container,
            [this, input_field, button_ok, button_cancel] {
                return window(text(*_title) | center,
                    vbox({
                        text(*_info),
                        input_field->Render() | border,
                        hbox({
                            button_ok->Render(),
                            button_cancel->Render(),
                        }),
                    })
                );
            }
        );

        return renderer;
    }

    bool
    correct() const
    { return _correct; }

    const std::string&
    content() const
    { return *_content; }

    void
    clear_content()
    { _content->clear(); }

    void
    clear_placeholder()
    { _placeholder->clear(); }

private:
    std::function<void()> check_content = [&] {
        _correct = validator(*_content);
    };
};


}

