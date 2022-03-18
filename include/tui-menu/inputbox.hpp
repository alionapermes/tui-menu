#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>
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
public:
    using InputPredicate = std::function<bool(const std::string&)>;

private: // fields
    size_t _min_len;
    size_t _max_len;
    std::string _buffer;
    std::string _placeholder;
    Component _input;
    Component _button_ok;
    Component _button_cancel;
    InputOption _input_option;

    OnEnterFunctor<Screen> _on_ok;
    OnEnterFunctor<Screen> _on_cancel;
    InputPredicate _predicate;

public: // ctors
    InputBox(
        std::string_view placeholder = "",
        size_t min_len               = 0,
        size_t max_len               = 0,
        InputPredicate predicate     = [](const std::string&) { return true; }
    )
        : _placeholder(placeholder)
        , _min_len(min_len)
        , _max_len(max_len)
        , _predicate(predicate)
    {
        std::function<void()> on_ok = [&] {
            if (isCorrect())
                _on_ok(dynamic_cast<Screen*>(_owner));
        };

        _input_option  = InputOption{ .on_enter = on_ok };
        _input         = Input(&_buffer, &_placeholder, &_input_option);

        _button_ok     = Button("ОК", on_ok);
        _button_cancel = Button("Отмена", [&] {
            _on_cancel(dynamic_cast<Screen*>(_owner));
        });
    }

public: // methods
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

    bool
    isCorrect() const
    {
        return (
            _predicate(_buffer)
            && (_buffer.length() >= _min_len)
            && ((_buffer.length() <= _max_len) || !_max_len)
        );
    }

    size_t
    getMinLen() const
    { return _min_len; }

    void
    setMinLen(size_t len)
    { _min_len = len; }

    size_t
    getMaxLen() const
    { return _max_len; }

    void
    setMaxLen(size_t len)
    { _max_len = len; }

    void
    setPredicate(InputPredicate predicate)
    { _predicate = predicate; }

public: // static validate predicats
    static bool
    isInteger(const std::string& buffer)
    {
        try {
            std::stoi(buffer);
            return true;
        } catch (std::invalid_argument&) {
            return false;
        }
    };

    static bool
    isFloat(const std::string& buffer)
    {
        try {
            std::stof(buffer);
            return true;
        } catch (std::invalid_argument&) {
            return false;
        }
    };

    static bool
    isNumeric(const std::string& buffer)
    { return isInteger(buffer) || isFloat(buffer); };
};

};

