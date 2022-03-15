#include "mainscreen.hpp"


void
unit1_handler(MainScreen* screen)
{ screen->UnitName = "ABOBUS POWER!!!"; }

void
unit2_handler(MainScreen* screen)
{ screen->UnitName = "бУбЫлДа"; }


int
main()
{
    MainScreen::Menu commands;
    MainScreen::Menu units;

    commands.addItems({
        {"command #1", [](MainScreen* screen) {
            screen->InfoBar = "[##....]";
        }},
        {"command #2", [](MainScreen* screen) {
            screen->InfoBar = "[####..]";
        }},
        {"command #3", [](MainScreen* screen) {
            screen->InfoBar = "[######]";
        }},
    });

    units.addItems({
        {"unit #1", &unit1_handler},
        {"unit #2", &unit2_handler},
    });

    MainScreen screen("title", &commands, &units);
    screen.render();

    return 0;
}

