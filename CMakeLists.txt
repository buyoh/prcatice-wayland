cmake_minimum_required(VERSION 3.0)
project(maiwayland CXX)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simple src/simple.cpp)
target_link_libraries(simple wayland-client pthread)

add_executable(mainapp
    src/mainapp.cpp
    src/wrapper/wayland_client.cpp
    src/wrapper/keyboard_listener.cpp
    src/wrapper/pointer_listener.cpp
    src/wrapper/registry_listener.cpp
    src/wrapper/shellsurface_listener.cpp
    src/wrapper/touch_listener.cpp
    src/wrapper/seat_listener.cpp
    )
target_link_libraries(mainapp wayland-client pthread xkbcommon)
