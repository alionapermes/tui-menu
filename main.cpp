#include "mainscreen.hpp"


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
        {"unit #1", [](MainScreen* screen) {
            screen->UnitName = "ABOBUS POWER!!!";
        }},
        {"unit #2", [](MainScreen* screen) {
            screen->UnitName = "бУбЫлДа";
        }},
    });

    MainScreen screen("title", &commands, &units);
    screen.render();

    return 0;
}

