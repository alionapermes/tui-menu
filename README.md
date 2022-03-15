# tui-menu

Консольный пользовательский интерфейс для управления контейнерами данных

## Сборка

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
make
```

## Запуск

Находясь в tui-menu/build:  
`./bin/menu`

