#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "ftxui/component/screen_interactive.hpp"
#include "tui-menu/mainwindow.hpp"


int
main()
{
    using namespace ftxui;

    tuim::MainWindow w("title");
    w.render();

    return 0;
}


