#pragma once

#include <functional>
#include <string>
#include <vector>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

#include "types.hpp"
#include "windowbase.hpp"


namespace tuim {

using namespace ftxui;


template <typename Key>
struct IGraphNode
{
    virtual std::vector<const Key*>
    keys() const = 0;

    virtual std::vector<const IGraphNode*>
    nodes() const = 0;
};

template <typename T>
using IGraphNodePtrs = std::vector<const tuim::IGraphNode<T>*>;


template <typename Key>
struct IGraphExportable
{
    virtual const IGraphNode<Key>*
    export_start_node() const = 0;
};


template <typename Key>
class GraphViewWindow : public WindowBase
{
private:
    const int _one = 20;
    IGraphExportable<Key>* _graph;
    Canvas _gv;
    
public:
    std::function<void()> on_exit;
    std::function<std::string (const IGraphNode<Key>*)> node_builder;

public:
    GraphViewWindow(string_like auto&& title = "")
        : WindowBase(std::forward<decltype(title)>(title))
    {}

    void
    set_graph(IGraphExportable<Key>* graph)
    { _graph = graph; }

    void
    init_canvas(int w, int h)
    { _gv = Canvas(w, h * 2); }

    void
    build_canvas()
    { build_graph(_graph->export_start_node(), 0, _gv.width()); }

private:
    Component
    renderer() override
    {
        Component renderer  = Renderer(Container::Horizontal({}), [this] {
            return window(text(_title) | center,
                vbox({
                    canvas(_gv) | flex,
                })
            );
        });

        return renderer;
    }

    void
    build_graph(const IGraphNode<Key>* node, int x0, int x1, int depth = 1)
    {
        std::string content = node_builder(node);

        int w    = x1 - x0;
        int y    = _one * depth;
        int step = w / (node->keys().size() + 1);
        int mid  = x0 + (w >> 1);
        int quad = _one >> 2;

        _gv.DrawText(mid - content.length(), y, std::move(content));


        int offset = x0 + step;
        for (auto sub : node->nodes()) {
            if (sub != nullptr) {
                int new_x0  = offset - step;
                int new_mid = new_x0 + (step >> 1);

                _gv.DrawPointLine(mid, y + quad, new_mid, y + _one - quad);
                build_graph(sub, offset - step, offset, depth + 1);

                offset += step;
            }
        }
    }
};


}

