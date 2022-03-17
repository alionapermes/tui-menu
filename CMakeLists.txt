cmake_minimum_required(VERSION 3.10)
project(tui-menu)

include_directories(${CMAKE_CURRENT_DIR})
include_directories(include)

include(FetchContent)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if (DEBUG)
    add_compile_options(-g3 -ggdb)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BIN menu)
set(SRC main.cpp)

add_executable(${BIN} ${SRC})
target_link_libraries(${BIN} ${CONAN_LIBS})

