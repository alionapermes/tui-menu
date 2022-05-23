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

#ifdef V3
    #define CHOOSING_NUMBER
    #define MODULE
    #define LINEAR
#endif
#ifdef V4
    #define HORNER
    #define MULTIPLICATIVE
    #define LINEAR
#endif
#ifdef V7
    #define CONVOLUTION
    #define MODULE
    #define QUADRATIC
#endif
#ifdef V8
    #define HORNER
    #define MULTIPLICATIVE
    #define LINEAR
#endif
#include "container.hpp"


class App
{
private: // aliases
#ifdef HORNER
    using value_type     = std::string;
#else
    using value_type     = uint64_t;
#endif
    using container_type = htab<value_type>;
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
#ifdef HORNER
        _input_window.set_placeholder("текст");
        _input_window.validator = tuim::wild_text();
    #ifdef V3
        _input_window.validator |= tuim::in_range(0, 1'000'000'000);
    #endif
    #ifdef V7
        _input_window.validator |= tuim::in_range(100'000'000, 300'000'000);
    #endif
#else
        _input_window.set_placeholder("целое число");
        _input_window.validator = tuim::is_integer();
#endif
        _input_window.on_ok     = [this] {
#ifdef HORNER
            std::string item    = _input_window.content();
            std::string message = "Элемент " + item;
#else
            int item            = std::stoi(_input_window.content());
            std::string message = "Элемент " + std::to_string(item);
#endif
            container_type& unit = _main_window.current_unit();

            try {
                auto& unit = _main_window.current_unit();
                message += (
                    unit.find(item) != unit.end()
                    ? " содержится в контейнере"
                    : " не найден"
                );
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
#ifdef HORNER
        _input_window.set_placeholder("текст");
        _input_window.validator = tuim::wild_text();
#else
        _input_window.set_placeholder("число");
        _input_window.validator = tuim::is_integer();
    #ifdef V3
        _input_window.validator |= tuim::in_range(0, 1'000'000'000);
    #endif
    #ifdef V7
        _input_window.validator |= tuim::in_range(100'000'000, 300'000'000);
    #endif
#endif
        _input_window.on_ok     = [this] {
            try {
#ifdef HORNER
                std::string value = _input_window.content();
#else
                uint64_t value = std::stoul(_input_window.content());
#endif
                auto iter = _main_window.current_unit().insert(value);
                if (iter == _main_window.current_unit().end())
                    _main_window.set_info("Элемент не может быть вставлен");
                else
                    _main_window.set_info(fmt::format(
                        "Вставлен элемент: {}", *iter
                    ));
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
#ifdef HORNER
        _input_window.set_placeholder("текст");
        _input_window.validator = tuim::wild_text();
#else
        _input_window.set_placeholder("число");
        _input_window.validator = tuim::is_integer();
    #ifdef V3
        _input_window.validator |= tuim::in_range(0, 1'000'000'000);
    #endif
    #ifdef V7
        _input_window.validator |= tuim::in_range(100'000'000, 300'000'000);
    #endif
#endif
        _input_window.on_ok     = [this] {
#ifdef HORNER
            std::string value = _input_window.content();
#else
            uint64_t value = std::stoul(_input_window.content());
#endif

            try {
                auto& unit = _main_window.current_unit();
                auto iter = unit.find(value);
                if (iter == unit.end())
                    _main_window.set_info("Нет такого элемента");
                else {
                    _main_window.current_unit().erase(iter);
                    _main_window.set_info(fmt::format(
                        "Элемент {} удалён", *iter
                    ));
                }
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
        _nav_window.on_change =
            [this](container_type::iterator it)
            {
                size_t index;
#ifdef LINEAR
                index = _main_window.current_unit().linear_find(*it);
#elif defined QUADRATIC
                index = _main_window.current_unit().quadratic_find(*it);
#endif
                std::string msg = fmt::format(
                    "original hash: {}; resolved: {}",
                    _main_window.current_unit().get_hash(*it),
                    index
                );
                _nav_window.set_info(msg);
            };
        _nav_window.set_container(&_main_window.current_unit());

        _tui.set_layer(_nwd);
    };

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


