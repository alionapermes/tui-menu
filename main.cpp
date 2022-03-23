#include <string>
#include <utility>
#include <vector>

#include "tui-menu/inputwindow.hpp"
#include "tui-menu/mainwindow.hpp"
#include "tui-menu/tui.hpp"
#include "tui-menu/validators.hpp"


int
main()
{
    tuim::TerminalUserInterface tui;
    tuim::MainWindow<std::vector<int>> mw("main window title");
    tuim::InputWindow iw_str("input window title", "len [1; 8]");
    tuim::InputWindow iw_num("number input", "type number");

    lid_t mwd     = tui.add_layer(&mw, true);
    lid_t iwd_str = tui.add_layer(&iw_str);
    lid_t iwd_num = tui.add_layer(&iw_num);

    mw.add_commands({
        {"new unit", [&] {
            iw_str.clear_content();
            tui.set_layer(iwd_str);
        }},
        {"add item", [&] {
            if (mw.units_empty())
                return mw.set_info("create/select a unit before");

            iw_num.clear_content();
            tui.set_layer(iwd_num);
        }},
        {"print items", [&] {
            std::string buffer;
            for (const auto& item : mw.current_unit())
                buffer += std::to_string(item) + ", ";

            mw.set_info(std::move(buffer));
        }},
        {"set info", [&] { mw.set_info("some useful info"); }},
    });

    iw_str.on_ok     = [&] {
        mw.add_unit(iw_str.content());
        tui.set_layer(mwd);
    };
    iw_str.on_cancel = [&] { tui.set_layer(mwd); };
    iw_str.validator = tuim::length_range(1, 9);

    iw_num.on_ok     = [&] {
        mw.current_unit().push_back(std::stoi(iw_num.content()));
        tui.set_layer(mwd);
    };
    iw_num.on_cancel = [&] { tui.set_layer(mwd); };
    iw_num.validator = tuim::is_numeric();

    tui.render();

    return 0;
}

