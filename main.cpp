#include "tui-menu/mainscreen.hpp"
#include "tui-menu/menu.hpp"
#include "tui-menu/window.hpp"

using namespace tuim;


void
unit1_handler(MainScreen* screen)
{ screen->setUnitName("ABOBUS POWER!!!"); }

void
unit2_handler(MainScreen* screen)
{ screen->setUnitName("бУбЫлДа"); }


int
main()
{
    tuim::Menu commands, units;
    MainScreen screen(&commands, &units);
    Window window("title", &screen);

    commands.addItems({
        {"command #1", [](MainScreen* screen) {
            screen->setInfoBar("[##....]");
        }},
        {"command #2", [](MainScreen* screen) {
            screen->setInfoBar("[####..]");
        }},
        {"command #3", [](MainScreen* screen) {
            screen->setInfoBar("[######]");
        }},
    });

    units.addItems({
        {"unit #1", &unit1_handler},
        {"unit #2", &unit2_handler},
    });

    window.render();

    return 0;
}

