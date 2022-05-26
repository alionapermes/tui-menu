#define RECURSIVE

#include "randtree-app.hpp"


int                                                                     
main()                                                                  
{                                                                       
    App app("ЛР №3, В-7. Баранова В., Обидиентов А.");

    app.add_commands({
        {"Создать контейнер",                   app.new_unit_empty   },
        {"Создать контейнер на основе другого", app.new_unit_based   },
        {"Вывод элементов контейнера",          app.read_items       },
        {"Количество элементов в контейнере",   app.items_count      },
        {"Проверка контейнера на пустоту",      app.unit_is_empty    },
        {"Очистка контейнера",                  app.unit_clear       },
        {"Проверка наличия элемента (поиск)",   app.contains         },
        {"Вставка элемента",                    app.insert           },
        {"Удаление элемента",                   app.erase            },
        {"Обзор дерева",                        app.output           },
        {"Навигация по контейнеру",             app.navigate         },
        {"Удаление контейнера",                 app.unit_remove      },
    });

    app.run();

    return 0;
}

