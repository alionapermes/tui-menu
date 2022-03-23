# tui-menu

Консольный пользовательский интерфейс (TUI) для управления контейнерами данных

Для навигации используются стрелки, выход по нажатию `q` или `Escape`

## Скрины

![demo1](images/demo1.png)

![demo2](images/demo2.png)

## Пример кода

![example](images/example.png)

## Сборка

`.h`/`.hpp` файл своего контейнера можно положить в каталог `include/`

Убедитесь, что у вас установлен менеджер пакетов [conan](https://conan.io)
и произведены его необходимые базовые настройки:

```sh
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default # linux
conan profile update settings.compiler.libcxx=libc++11 default    # macos
```

Дополнительные флаги сборки:

 * -DDEBUG=1 - для [gdb](https://www.sourceware.org/gdb/)

 * -DCMAKE_EXPORT_COMPILE_COMMANDS=1 - помогает редактору кода увидеть
 библиотеки и файлы

```sh
mkdir build
cd build
conan install ..
cmake ..
cmake --build .
```

## Запуск

Находясь в build:  
`./bin/menu`

## API

Краткое описание возможностей библиотеки

### Концепты

```cpp
template<typename T>
concept string_like = std::is_convertible_v<T, std::string_view>;
```

`string_like` ограничивает шаблонный параметр `T` типами,
конвертируемыми в строковый вид

### Класс TerminalUserInterface

Основной класс, отвечающий за работу программы

#### Типы

`lid_t` - тип дескриптора окна

#### Методы

```cpp
lid_t
add_layer(WindowBase* layer, bool as_main = false)
```

Возвращает дескриптор добавленного окна

---

```cpp
void
set_layer(lid_t lid)
```

Устанавливает слой для рендера

---

```cpp
set_main_layer(lid_t lid)
```

Устанавливает основной слой рендера

---

```cpp
void
render()
```

Метод, запускающий обработку интерфейса

---

### Класс MainWindow

Класс, позволяющий управлять содержимым меню команд и контейнеров

#### Типы

```cpp
template <string_like Text>
using ItemsList<Text> = std::vector<std::pair<std::string, std::function<void()>>>;
```

#### Методы

```cpp
MainWindow(string_like auto&& title)
```

Конструктор, устанавливающий заголовок окна

---

```cpp
void
set_info(string_like auto&& text)
```

Установка поля для вывода информации

![field info](images/info.png)

---

```cpp
void
set_unit(string_like auto&& text)
```

Установка поля для вывода названия контейнера

![field unit](images/unit.png)

---

```cpp
const std::string&
selected_command() const
```

Возвращает название выбранной в данной момент команды

---

```cpp
const std::string&
selected_unit() const
```

Возвращает название выбранного в данный момент контейнера

---

```cpp
template <string_like Text = std::string>
void
add_commands(ItemsList<Text> commands)
```

Добавляет спиок команд с их обработчиками

---

### Класс InputWindow

Класс для чтения пользовательского ввода

#### Методы

```cpp
InputWindow(
	string_like auto&& title,
	string_like auto&& placeholder
)
```

Конструктор, устанавливающий заголовок окна и плейсхолдер поля ввода

---

```cpp
bool
correct() const
```

Определяет корректность содержимого поля ввода

---

```cpp
const std::string&
content() const
```

Возвращает содержимое поля ввода

---

```cpp
void
clear_placeholder()
```

Очищает плейсхолдер поля ввода

---

```cpp
void
clear_content()
```

Очищает содержимое поля ввода

---

#### Обработчики

```cpp
std::function<bool(std::string_view)> validator
```

Обработчик, неявно принимающий в качестве единственного аргумента содержимое поля ввода и определяет его корректность.

---

```cpp
std::function<void()> on_ok
```

Отвечает за обработку нажатия кнопки ОК или Enter на поле ввода

---

```cpp
std::function<void()> on_cancel
```

Отвечает за обработку нажатия кнопки Отмена

---
