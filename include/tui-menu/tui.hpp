#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "windowbase.hpp"
#include "types.hpp"


namespace tuim {

using namespace ftxui;


class TerminalUserInterface
{
private: // fields
    lid_t _lid_current = 0;
    lid_t _lid_main    = 0;
    std::vector<WindowBase*> _layers;
    ScreenInteractive _screen = ScreenInteractive::TerminalOutput();

public: // ctors
    TerminalUserInterface() = default;

public: // methods
    lid_t
    add_layer(WindowBase* layer, bool as_main = false)
    {
        _layers.push_back(layer);

        lid_t id = _layers.size() - 1;
        if (as_main)
            _lid_main = id;

        return id;
    }

    void
    set_layer(lid_t lid)
    { _lid_current = lid; }

    void
    set_main_layer(lid_t lid)
    { _lid_main = lid; }

    void
    render()
    {
        Components renderers;
        for (auto& layer : _layers)
            renderers.push_back(layer->renderer());

        auto main_container = Container::Tab(renderers, &_lid_current);
        auto main_renderer  = Renderer(main_container,
            [this, renderers = std::move(renderers)] {
                Element document = renderers[_lid_main]->Render();

                if (_lid_current != _lid_main) {
                    document = dbox({
                        document,
                        renderers[_lid_current]->Render()
                            | clear_under | center,
                    });
                }

                return document;
            }
        );

        _screen.Loop(main_renderer);
    }
};


}

