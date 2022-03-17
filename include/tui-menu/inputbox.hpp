#pragma once

#include <functional>
#include <string>
#include <string_view>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"

#include "types.hpp"
#include "combinedcomponent.hpp"


namespace tuim {

using namespace ftxui;

class InputScreen;


template <typename Screen = InputScreen>
class InputBox : public CombinedComponent
{
private:
    std::string _buffer;
    std::string _placeholder;
    Component _input;
    Component _button_ok;
    Component _button_cancel;
    InputOption _input_option;

    OnEnterFunctor<Screen> _on_ok;
    OnEnterFunctor<Screen> _on_cancel;

public:
    InputBox()
    {
        std::function<void()> on_ok = [&] {
            _on_ok(dynamic_cast<Screen*>(_owner));
        };

        _input_option  = InputOption{ .on_enter = on_ok };
        _input         = Input(&_buffer, &_placeholder, &_input_option);

        _button_ok     = Button("ОК", on_ok);
        _button_cancel = Button("Отмена", [&] {
            _on_cancel(dynamic_cast<Screen*>(_owner));
        });
    }

    Component
    component() override
    { return _input; }

    Component
    buttonOk() const
    { return _button_ok; }

    Component
    buttonCancel() const
    { return _button_cancel; }

    void
    onOk(OnEnterFunctor<Screen> handler)
    { _on_ok = handler; }

    void
    onCancel(OnEnterFunctor<Screen> handler)
    { _on_cancel = handler; }

    std::string_view
    content() const
    { return _buffer; }

    void
    clear()
    { _buffer.clear(); }
};

};

