#include "tui-menu/inputwindow.hpp"
#include "tui-menu/mainwindow.hpp"
#include "tui-menu/tui.hpp"
#include "tui-menu/validators.hpp"


int
main()
{
    tuim::TerminalUserInterface tui;
    tuim::MainWindow mw("main window title");
    tuim::InputWindow iw("input window title", "type something");

    lid_t mwd = tui.add_layer(&mw, true);
    lid_t iwd = tui.add_layer(&iw);

    mw.add_commands({
        {"command #1", [&] { tui.set_layer(iwd); }},
        {"command #2", [&] { mw.set_unit("unit !!"); }},
        {"command #3", [&] { mw.set_info("some useful info"); }},
    });

    iw.on_ok = [&] {
        mw.set_title(iw.content());
        tui.set_layer(mwd);
    };
    iw.on_cancel = [&] { tui.set_layer(mwd); };
    iw.validator = &tuim::is_numeric;

    tui.render();

    return 0;
}

