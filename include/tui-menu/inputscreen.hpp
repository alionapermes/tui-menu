#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

#include "iscreen.hpp"
#include "inputbox.hpp"
#include "types.hpp"


namespace tuim {

using namespace ftxui;


class InputScreen : public IScreen
{
private:
    std::string _info_bar;
    std::string _incorrect_msg;
    InputBox<InputScreen>* _input_box = nullptr;

public:
    InputScreen(InputBox<InputScreen>* input_box) : _input_box(input_box)
    { _input_box->_owner = this; }

private:
    Element
    document() override
    {
        return vbox({
            text(((_input_box->isCorrect() && !_input_box->content().empty())
                ? _info_bar
                : (_info_bar + "\n[" + _incorrect_msg + ']')
            )) | center,
            _input_box->component()->Render() | border | flex,
            hbox({
                _input_box->buttonOk()->Render() | flex,
                _input_box->buttonCancel()->Render() | flex,
            }),
        });
    }

    Component
    container() override
    {
        return Container::Vertical({
            _input_box->component(),
            Container::Horizontal({
                _input_box->buttonOk(),
                _input_box->buttonCancel(),
            }),
        });
    }

public:
    std::string_view
    getInfoBar() const
    { return _info_bar; }

    void
    setInfoBar(StringLike auto&& text)
    { _info_bar = std::forward<decltype(text)>(text); }

    std::string_view
    getIncorrectMessage() const
    { return _incorrect_msg; }

    void
    setIncorrectMessage(StringLike auto&& text)
    { _incorrect_msg = std::forward<decltype(text)>(text); }
};

};

