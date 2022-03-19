#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "tui-menu/inputbox.hpp"
#include "tui-menu/inputscreen.hpp"
#include "tui-menu/mainscreen.hpp"
#include "tui-menu/navscreen.hpp"
#include "tui-menu/tui-menu.hpp"
#include "tui-menu/window.hpp"

/* #include "container.hpp" */

using namespace tuim;
using Container  = std::vector<int>;
using Containers = std::vector<::Container>;


Containers containers;

std::unique_ptr<Window> w;

int num_input_d, str_input_d, nav_d;
MainScreen::Menu commands, units;

MainScreen main_screen(&commands, &units);
NavScreen nav_screen;

InputBox num_input("число"), str_input("введите текст до 4 символов", 0, 4);
InputScreen num_input_screen(&num_input), str_input_screen(&str_input);


void
itemAdd(InputScreen* screen)
{
    int value = std::stoi(std::string(num_input.content()));
    containers[units.selected()].push_back(value);
    w->switchScreen(0);
}

void
printItems(MainScreen* screen)
{
    std::stringstream ss;
    ss << "{";
    for (const auto& item : containers[units.selected()])
        ss << item << ", ";
    ss << "}";
    screen->setInfoBar(ss.str());
}

void
navCurrent(MainScreen* screen)
{
    //
}

void
addContainer(InputScreen* screen)
{
    std::string unit_name = std::string(str_input.content());

    containers.push_back(::Container());
    units.addItem(unit_name, [&](MainScreen* screen) {
        screen->setUnitName("Контейнер: " + unit_name);
        screen->setInfoBar("");
        units.select(units.size() - 1);
    });

    w->switchScreen(0);
}


int
main()
{
    num_input.setPredicate(&num_input.isInteger);
    num_input_screen.setIncorrectMessage("Ошибка ввода");
    str_input_screen.setIncorrectMessage("ит'с соу биг O_O");

    num_input.onOk(&itemAdd);
    num_input.onCancel([&](InputScreen*) { w->switchScreen(0); });
    str_input.onOk(&addContainer);
    str_input.onCancel([&](InputScreen*) { w->switchScreen(0); });

    commands.addItems({
        {"Добавить элемент в контейнер", [&](MainScreen*) {
            num_input.clear();
            w->switchScreen(num_input_d);
        }},
        {"Вывод элементов контейнера", &printItems},
        {"Навигация по контейнеру", &navCurrent},
        {"Новый контейнер", [&](MainScreen*) {
            str_input.clear();
            w->switchScreen(str_input_d);
        }},
        {"Выход", [](MainScreen*) { w->exit(); }},
    });

    w = std::make_unique<Window>(
        "Демо вариант / tui-menu beta",
        &main_screen
    );
    num_input_d = w->addModal(&num_input_screen);
    str_input_d = w->addModal(&str_input_screen);
    nav_d       = w->addModal(&nav_screen);

    w->render();

    return 0;
}


