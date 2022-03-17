#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

#include "iscreen.hpp"
#include "types.hpp"


namespace tuim {

using namespace ftxui;


class NavScreen : public IScreen
{
private:
    std::string _content;
    Component _button_prev;
    Component _button_next;
    Component _button_exit;

    OnEnterFunctor<NavScreen> _on_prev;
    OnEnterFunctor<NavScreen> _on_next;
    OnEnterFunctor<NavScreen> _on_exit;

public:
    NavScreen()
    {
        _button_prev = Button("Предыдущий", [&] { _on_prev(this); });
        _button_next = Button("Следующий", [&] { _on_next(this); });
        _button_exit = Button("Назад", [&] { _on_exit(this); });
    }

private:
    Element
    document() override
    {
        return vbox({
            hbox({
                _button_prev->Render(),
                center(text(_content)) | border | flex,
                _button_next->Render(),
            }),
            _button_exit->Render() | flex,
        });
    }

    Component
    container() override
    {
        return Container::Vertical({
            Container::Horizontal({
                _button_prev,
                _button_next
            }),
            _button_exit,
        });
    }

public:
    std::string_view
    getContent() const
    { return _content; }

    template <StringLike Text>
    void
    setContent(Text&& content)
    { _content = std::forward<Text>(content); }

    void
    onNext(OnEnterFunctor<NavScreen> handler)
    { _on_next = handler; }

    void
    onPrev(OnEnterFunctor<NavScreen> handler)
    { _on_prev = handler; }

    void
    onExit(OnEnterFunctor<NavScreen> handler)
    { _on_exit = handler; }
};

};

