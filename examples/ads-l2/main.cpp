/* #define TLR */
#define LRT
/* #define LTR */
#define ITERATIVE
/* #define RECURSIVE */
/* #define COUNT_MORE_THAN */
/* #define BALANCE_FACTOR */
/* #define INDEX_OF */
#define MERGE

#ifdef TLR
    #define SCHEME "t -> L -> R"
#elif defined LRT
    #define SCHEME "L -> R -> t"
#elif defined LTR
    #define SCHEME "L -> t -> R"
#endif

#include "app.hpp"


int                                                                     
main()                                                                  
{                                                                       
    App app("ЛР №2, В-8. Шаповаленко В., Попов П.");

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
        {"Вывод по схеме " SCHEME,              app.output           },
#ifdef COUNT_MORE_THAN
        {"Число элементов больше заданного",    app.count_more_than  },
#endif
#ifdef BALANCE_FACTOR
        {"Критерий сбалансированности",         app.balance_factor   },
#endif
#ifdef INDEX_OF
        {"Порядковый номер элемента",           app.index_of         },
#endif
#ifdef MERGE
        {"Объединение двух деревьев",           app.merge            },
#endif
        {"Навигация по контейнеру",             app.navigate         },
        {"Удаление контейнера",                 app.unit_remove      },
    });

    app.run();

    return 0;
}

