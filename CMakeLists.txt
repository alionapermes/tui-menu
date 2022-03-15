cmake_minimum_required(VERSION 3.10)
project(main)

include(FetchContent)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if (DEBUG)
    add_compile_options(-g3 -ggdb)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXEC menu)
set(SRC main.cpp)

add_executable(${EXEC} ${SRC})
target_link_libraries(${EXEC} ${CONAN_LIBS})

