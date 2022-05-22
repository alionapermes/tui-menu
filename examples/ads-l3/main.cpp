/* #define ITERATIVE */
#define RECURSIVE

#include "app.hpp"


int                                                                     
main()                                                                  
{                                                                       
    App app("ЛР №4, В-5. Москвичёв С., Розин М.");

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

