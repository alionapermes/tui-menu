/* #define V4 */
/* #define V5 */
/* #define V7 */
#define V8
#include "app.hpp"


int                                                                     
main()                                                                  
{                                                                       
    App app("ЛР №4, В-8. Москвин Т., Хейнов В.");

    app.add_commands({
        {"Создать контейнер",                   app.new_unit_empty   },
        {"Вывод элементов контейнера",          app.read_items       },
        {"Количество элементов в контейнере",   app.items_count      },
        {"Проверка контейнера на пустоту",      app.unit_is_empty    },
        {"Очистка контейнера",                  app.unit_clear       },
        {"Проверка наличия элемента (поиск)",   app.contains         },
        {"Вставка элемента",                    app.insert           },
        {"Удаление элемента",                   app.erase            },
        {"Навигация по контейнеру",             app.navigate         },
        {"Удаление контейнера",                 app.unit_remove      },
    });

    app.run();

    return 0;
}

