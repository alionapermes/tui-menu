#include <cstdlib>
#include <string>

#include "tui-menu/navscreen.hpp"
#include "tui-menu/tui-menu.hpp"

using namespace tuim;


int
main()
{
    MainScreen::Menu commands, units;
    InputBox input_box("Введите число");//, 0, 10, &isNumeric);
    /* input_box.setPredicate(&input_box.isNumeric); */

    MainScreen main_screen(&commands, &units);
    InputScreen input_screen(&input_box);
    input_screen.setIncorrectMessage("тобi пизда");
    NavScreen nav_screen;

    Window window("title", &main_screen);
    nav_screen.onExit([&](NavScreen*) { window.switchScreen(0); });
    int inp1 = window.addModal(&input_screen);
    int nav1 = window.addModal(&nav_screen);

    commands.addItems({
        {"Добавить контейнер", [&](MainScreen* screen) {
            input_box.clear();
            window.switchScreen(inp1);
        }},
        {"Навигация по контейнеру", [&](MainScreen* screen) {
            window.switchScreen(nav1);
        }},
        {"Выход", [&](MainScreen*) { window.exit(); }},
    });

    input_box.onOk([&](InputScreen* screen) {
        std::string name = std::string(input_box.content());

        if (!name.empty()) {
            if (!std::atoi(name.c_str())) {
                units.addItem(name, [&](MainScreen* screen) {
                    screen->setUnitName(units.selected());
                });
            }
            window.switchScreen(0);
        }
    });
    input_box.onCancel([&](InputScreen*) { window.switchScreen(0); });

    window.render();

    return 0;
}

