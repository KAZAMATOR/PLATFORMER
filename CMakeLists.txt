cmake_minimum_required(VERSION 3.20)
project(platformer)

set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR C:/SFML/SFML-2.5.1)
set(SFML_ROOT ${SFML_DIR})
set(CMAKE_MODULE_PATH "${SFML_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5.1 COMPONENTS system window graphics audio network REQUIRED)
include_directories(C:/SFML/SFML-2.5.1/include)
add_executable(platformer
        main.cpp
        namespace_header.h maps.h classes2.0.h classes2.0.cpp help_functions.h felp_functions.cpp)
target_link_libraries(platformer sfml-graphics sfml-system sfml-window)