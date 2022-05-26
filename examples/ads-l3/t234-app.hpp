#pragma once

#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "fmt/core.h"

#include "tui-menu/graphviewwindow.hpp"
#include "tui-menu/tui.hpp"
#include "tui-menu/inputwindow.hpp"
#include "tui-menu/mainwindow.hpp"
#include "tui-menu/navwindow.hpp"
#include "tui-menu/types.hpp"
#include "tui-menu/validators.hpp"

#include "t234.hpp"


class App
{
private: // aliases
    using value_type     = int;
    using container_type = t234<value_type>;
    using ItemsList      = tuim::MainWindow<container_type>::ItemsList;

private: // fields
    tuim::TerminalUserInterface _tui;
    tuim::MainWindow<container_type> _main_window;
    tuim::NavWindow<container_type> _nav_window;
    tuim::GraphViewWindow<value_type> _gv_window;
    tuim::InputWindow _input_window;
    lid_t _mwd, _iwd, _nwd, _gwd;

public: // ctors
    App(tuim::string_like auto&& title)
        : _main_window(std::forward<decltype(title)>(title))
        , _nav_window("Навигация")
        , _gv_window("Обзор контейнера")
        , _input_window("Ввод")
    {}

public: // methods
    void
    run()
    {
        _mwd = _tui.add_layer(&_main_window, true);
        _iwd = _tui.add_layer(&_input_window);
        _gwd = _tui.add_layer(&_gv_window);
        _nwd = _tui.add_layer(&_nav_window);

        _input_window.on_cancel = [this] { _tui.set_layer(_mwd); };
        _gv_window.on_exit      = [this] { _tui.set_layer(_mwd); };
        _nav_window.on_exit     = [this] { _tui.set_layer(_mwd); };

        _tui.render();
    }

    void
    add_commands(ItemsList&& commands)
    { _main_window.add_commands(std::move(commands)); }

    std::function<void()>
    new_unit_empty = [this]
    {
        _input_window.reset();
        _input_window.set_title("Новый контейнер");
        _input_window.set_placeholder("название");
        _input_window.validator = tuim::wild_text();
        _input_window.on_ok     = [this] {
            _main_window.add_unit(_input_window.content());
            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    new_unit_based = [this]
    {
        _input_window.reset();
        _input_window.set_title(_main_window.selected_command());
        _input_window.set_placeholder("имя источника");
        _input_window.validator = tuim::wild_text();
        _input_window.on_ok = [this] {
            auto unit = _main_window.get_unit(_input_window.content());

            if (!unit) {
                _input_window.set_info("Такого контейнера нет");
            } else {
                _input_window.reset();
                _input_window.set_title("Новый контейнер");
                _input_window.on_ok = [this, unit] {
                    _main_window.add_unit(_input_window.content(), *unit);
                    _tui.set_layer(_mwd);
                };
            }
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    read_items = [this]
    {
        try {
            std::string str = "{ ";
            for (const auto& item : _main_window.current_unit())
                str += fmt::format("{}, ", item);
            str += "}";

            _main_window.set_info(str);
        } catch (const std::runtime_error& re) {
            _main_window.set_info("Ошибка: " + std::string(re.what()));
        }
    };

    std::function<void()>
    items_count = [this]
    {
        std::string message;

        try {
            size_t count = _main_window.current_unit().size();
            message      = "Количество элементов: " + std::to_string(count);
        } catch (const std::runtime_error& re) {
            message = "Ошибка: " + std::string(re.what());
        }

        _main_window.set_info(std::move(message));
    };

    std::function<void()>
    unit_is_empty = [this]
    {
        std::string message = "Контейнер ";

        try {
            message += (
                _main_window.current_unit().empty()
                ? "пуст"
                : "не пуст"
            );
        } catch (const std::runtime_error& re) {
            message = "Ошибка: " + std::string(re.what());
        }

        _main_window.set_info(std::move(message));
    };

    std::function<void()>
    unit_clear = [this]
    {
        try {
            _main_window.current_unit().clear();
            _main_window.set_info("Контейнер очищен");
        } catch (const std::runtime_error& re) {
            _main_window.set_info("Ошибка: " + std::string(re.what()));
        }
    };

    std::function<void()>
    contains = [this]
    {
        if (_main_window.units_empty())
            return _main_window.set_info("Ошибка: нет контейнера");

        _input_window.reset();
        _input_window.set_title("Поиск элемента");
        _input_window.set_placeholder("целое число");
        _input_window.validator = tuim::is_integer();
        _input_window.on_ok     = [this] {
            int item             = std::stoi(_input_window.content());
            std::string message  = "Элемент " + std::to_string(item);
            container_type& unit = _main_window.current_unit();

            try {
                /* message += ( */
                /*     _main_window.current_unit().contains(item) */
                /*     ? " содержится в контейнере" */
                /*     : " не найден" */
                /* ); */
            } catch (const std::runtime_error& re) {
                message = "Ошибка: " + std::string(re.what());
            }

            _main_window.set_info(std::move(message));
            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    insert = [this]
    {
        _input_window.reset();
        _input_window.set_title("Вставка элемента");
        _input_window.set_placeholder("число");
        _input_window.validator = tuim::is_integer();
        _input_window.on_ok     = [this] {
            try {
                int value = std::stoi(_input_window.content());
                _main_window.current_unit().insert(value);
            } catch (const std::runtime_error& re) {
                _main_window.set_info("Ошибка: " + std::string(re.what()));
            } catch (const std::invalid_argument& ia) {
                _main_window.set_info("Неверный ввод!");
            }

            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    erase = [this]
    {
        _input_window.reset();
        _input_window.set_title("Удаление элемента");
        _input_window.set_placeholder("число");
        _input_window.validator = tuim::is_integer();
        _input_window.on_ok     = [this] {
            int value = std::stoi(_input_window.content());

            try {
                auto& unit = _main_window.current_unit();
                auto iter = unit.find(value);
                if (iter == unit.end())
                    _main_window.set_info("Нет такого элемента");
                else
                    _main_window.current_unit().erase(iter);
            } catch (const std::runtime_error& re) {
                _main_window.set_info("Ошибка: " + std::string(re.what()));
            }

            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    unit_remove = [this]
    {
        _input_window.reset();
        _input_window.set_title("Удаление контейнера");
        _input_window.set_placeholder("название");
        _input_window.validator = tuim::wild_text();
        _input_window.on_ok     = [this] {
            const std::string& name = _input_window.content();
            std::string message     = "Контейнер " + name;

            message += (
                _main_window.remove_unit(name)
                ? " удалён"
                : " не найден"
            );
            
            _main_window.set_info(std::move(message));
            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    navigate = [this]
    {
        if (_main_window.current_unit().empty())
            return _main_window.set_info("Контейнер пуст");

        _nav_window.reset();
        _nav_window.set_container(&_main_window.current_unit());

        _tui.set_layer(_nwd);
    };

#ifdef COUNT_MORE_THAN
    std::function<void()>
    count_more_than = [this]
    {
        _input_window.reset();
        _input_window.set_title("Число элементов больше заданного");
        _input_window.set_placeholder("число");
        _input_window.validator = tuim::is_integer();
        _input_window.on_ok     = [this] {
            int value = std::stoi(_input_window.content());

            try {
                size_t n = _main_window.current_unit().count_more_than(value);
                _main_window.set_info(
                    "Число элементов больше заданного: " + std::to_string(n)
                );
            } catch (const std::runtime_error& re) {
                _main_window.set_info("Ошибка: " + std::string(re.what()));
            }

            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };
#endif

#ifdef BALANCE_FACTOR
    std::function<void()>
    balance_factor = [this]
    {
        try {
            std::string info;
            auto& unit = _main_window.current_unit();

            for (auto iter = unit.begin(); iter != unit.end(); ++iter) {
                ssize_t bf = unit.balance_factor(iter);
                info += fmt::format("{}: {}; ", *iter, bf);
            }

            _main_window.set_info(info);
        } catch (const std::runtime_error& re) {
            _main_window.set_info("Ошибка: " + std::string(re.what()));
        }
    };
#endif

    std::function<void()>
    output = [this]
    {
        auto builder = [](const tuim::IGraphNode<value_type>* node) {
            std::string str;
            auto keys = node->keys();

            for (size_t n = 0; n < keys.size(); ++n)
                str += fmt::format("| {} ", *keys[n]);

            return str + "|";
        };

        _gv_window.node_builder = builder;
        _gv_window.init_canvas(_tui.width(), _tui.height());
        _gv_window.set_graph(&_main_window.current_unit());
        _gv_window.build_canvas();

        _tui.set_layer(_gwd);
    };

#ifdef INDEX_OF
    std::function<void()>
    index_of = [this]
    {
        _input_window.reset();
        _input_window.set_title("Порядковый номер элемента");
        _input_window.set_placeholder("число");
        _input_window.validator = tuim::is_integer();
        _input_window.on_ok     = [this] {
            int value = std::stoi(_input_window.content());

            try {
                auto& unit = _main_window.current_unit();
                size_t n   = unit.index_of(value) + 1;
                if (n > unit.size())
                    _main_window.set_info("Нет такого элемента");
                else {
                    _main_window.set_info(fmt::format(
                        "Порядковый номер элемента {} равен {}",
                        value, n
                    ));
                }
            } catch (const std::runtime_error& re) {
                _main_window.set_info("Ошибка: " + std::string(re.what()));
            }

            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };
#endif

#ifdef MERGE
    std::function<void()>
    merge = [this]
    {
        _input_window.reset();
        _input_window.set_title("Объединение контейнеров");
        _input_window.set_placeholder("имя второго контейнера");
        _input_window.validator = tuim::wild_text();
        _input_window.on_ok     = [this] {
            const std::string& name = _input_window.content();
            std::string message     = "Контейнер " + name;
            
            try {
                auto src_unit = _main_window.get_unit(name);
                if (src_unit == nullptr)
                    message = "Нет такого контейнера";
                else
                    _main_window.current_unit().merge(*src_unit);
            } catch (const std::runtime_error& re) {
                message = fmt::format("Ошибка: {}", re.what());
            }

            _main_window.set_info(std::move(message));
            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };
#endif

#ifdef GREATER_TO_ROOT
    std::function<void()>
    greater_to_root = [this]
    {
        _input_window.reset();
        _input_window.set_title("Ввод элемента");
        _input_window.set_placeholder("число");
        _input_window.validator = tuim::is_integer();
        _input_window.on_ok     = [this] {
            int value = std::stoi(_input_window.content());

            try {
                auto& unit = _main_window.current_unit();
                auto iter  = unit.greater_to_root(value);

                if (iter == unit.end())
                    _main_window.set_info("Элемента больше заданного нет");
                else {
                    _main_window.set_info(fmt::format(
                        "Теперь корневой элемент равен {}", *iter
                    ));
                }
            } catch (const std::runtime_error& re) {
                _main_window.set_info("Ошибка: " + std::string(re.what()));
            }

            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };
#endif

private:
    void
    by_index_setup(tuim::string_like auto&& title)
    {
        using namespace tuim;
        container_type* unit = &_main_window.current_unit();

        _input_window.reset();
        _input_window.set_title(std::forward<decltype(title)>(title));
        _input_window.set_placeholder(fmt::format(
            "индекс [0; {})", unit->size()
        ));
        _input_window.validator = is_integer() | in_range(0, unit->size() - 1);
    };

    void
    by_name_setup(
        tuim::string_like auto&& title,
        tuim::string_like auto&& placeholder
    ) {
        using namespace tuim;
        container_type* unit = &_main_window.current_unit();

        _input_window.hard_reset();
        _input_window.set_title(std::forward<decltype(title)>(title));
        _input_window.set_placeholder(
            std::forward<decltype(placeholder)>(placeholder)
        );
    };
};


