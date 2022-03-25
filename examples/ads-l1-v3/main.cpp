#include "app.hpp"


int
main()
{
    App app("ЛР №1, В-3. Баранова В., Обидиентов А.");

    app.add_commands({
        {"Создать контейнер",                   app.new_unit_empty   },
        {"Создать контейнер с резервированием", app.new_unit_reserved},
        {"Создать контейнер на основе другого", app.new_unit_based   },
        {"Получить элемент по индексу",         app.read_by_index    },
        {"Записать элемент по индексу",         app.write_by_index   },
        {"Вывод элементов контейнера",          app.read_items       },
        {"Количество элементов в контейнере",   app.items_count      },
        {"Объём контейнера",                    app.unit_capacity    },
        {"Проверка контейнера на пустоту",      app.unit_is_empty    },
        {"Очистка контейнера",                  app.unit_clear       },
        /* {"Добавление в конец контейнера",       app.push_back        }, */
        /* {"Добавление в начало контейнера",      app.push_front       }, */
        {"Проверка наличия элемента (поиск)",   app.contains         },
        /* {"Вставка перед позицией",              app.insert_before    }, */
        /* {"Удаление позиции",                    app.erase            }, */
        {"Удаление контейнера",                 app.unit_remove      },
    });

    app.run();

    return 0;
}

