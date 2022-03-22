#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "ftxui/component/screen_interactive.hpp"
#include "tui-menu/inputwindow.hpp"
#include "tui-menu/mainwindow.hpp"
#include "tui-menu/tui.hpp"
#include "tui-menu/types.hpp"


int
main()
{
    using namespace ftxui;

    tuim::TUI term;
    tuim::MainWindow mw("main window title");
    tuim::InputWindow iw("input window title", "type something");

    lid_t mwd = term.add_layer(&mw, true);
    lid_t iwd = term.add_layer(&iw);

    mw.on_enter_units    = [&] {
        mw.set_unit(mw.selected_unit());
        term.set_layer(iwd);
    };
    mw.on_enter_commands = [&] {
        mw.set_info(mw.selected_command());
        term.set_layer(iwd);
    };

    iw.on_ok = [&] {
        mw.set_title(iw.content());
        term.set_layer(mwd);
    };
    iw.on_cancel = [&] { term.set_layer(mwd); };

    term.render();

    return 0;
}


