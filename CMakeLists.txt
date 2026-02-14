cmake_minimum_required(VERSION 3.16)
project(SimpleInput)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "/Users/apple/Qt/6.10.1/macos")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(SimpleInput main.cpp)

target_link_libraries(SimpleInput PRIVATE Qt6::Widgets)

