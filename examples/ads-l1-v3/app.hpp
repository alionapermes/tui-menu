#pragma once

#include <stdexcept>
#include <string>
#include <utility>

#include "tui-menu/tui.hpp"
#include "tui-menu/inputwindow.hpp"
#include "tui-menu/mainwindow.hpp"
#include "tui-menu/types.hpp"
#include "tui-menu/validators.hpp"

#include "container.hpp"


class App
{
private: // aliases
    using container_type = list_v3<int>;
    using ItemsList      = tuim::MainWindow<container_type>::ItemsList;

private: // fields
    tuim::TerminalUserInterface _tui;
    tuim::MainWindow<container_type> _main_window;
    tuim::InputWindow _input_window;
    lid_t _mwd, _iwd;

public: // ctors
    App(tuim::string_like auto&& title)
        : _main_window(title)
        , _input_window(tuim::InputWindow("Ввод")) {}

public: // methods
    void
    run()
    {
        _mwd = _tui.add_layer(&_main_window, true);
        _iwd = _tui.add_layer(&_input_window);

        _input_window.on_cancel = [this] { _tui.set_layer(_mwd); };

        _tui.render();
    }

    void
    add_commands(ItemsList&& commands)
    { _main_window.add_commands(std::move(commands)); }

    std::function<void()>
    new_unit_empty = [this]
    {
        _input_window.reset();
        _input_window.set_placeholder("имя контейнера");
        _input_window.on_ok = [this] {
            _main_window.add_unit(_input_window.content());
            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    new_unit_reserved = [this]
    {
        using namespace tuim;

        _input_window.reset();
        _input_window.set_title("Новый контейнер. Объём");
        _input_window.set_placeholder("объём");
        _input_window.validator = is_integer() | positive() | not_null();
        _input_window.on_ok     = [this] {
            int capacity = std::stoi(_input_window.content());

            _input_window.reset();
            _input_window.set_title("Новый контейнер. Название");
            _input_window.set_info("Объём: " + std::to_string(capacity));
            _input_window.set_placeholder("имя контейнера");
            _input_window.validator = wild_text();

            _input_window.on_ok = [this, capacity] {
                _main_window.add_unit(_input_window.content(), capacity);
                _tui.set_layer(_mwd);
            };
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    new_unit_based = [this]
    {
        _input_window.reset();
        _input_window.set_title("Базовый контейнер");
        _input_window.set_placeholder("имя контейнера");
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
    read_by_index = [this]
    {
        //
    };

    std::function<void()>
    write_by_index = [this]
    {
        //
    };

    std::function<void()>
    read_items = [this]
    {
        bool first          = true;
        std::string message = "{ ";

        try {
            for (const auto& item : _main_window.current_unit()) {
                if (first)
                    first = false;
                else
                    message += ", ";

                message += item;
            }

            message += " }";
        } catch (const std::runtime_error& re) {
            message = "Ошибка: " + std::string(re.what());
        }

        _main_window.set_info(std::move(message));
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
    unit_capacity = [this]
    {
        std::string message;

        try {
            size_t cap = _main_window.current_unit().capacity();
            message    = "Объём контейнера: " + std::to_string(cap);
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
    push_back = [this]
    {
        //
    };

    std::function<void()>
    push_front = [this]
    {
        //
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
                message += (
                    unit.find(item) == unit.end()
                    ? " не найден"
                    : " содержится в контейнере"
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
    insert_before = [this]
    {
        //
    };

    std::function<void()>
    erase = [this]
    {
        //
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
};

