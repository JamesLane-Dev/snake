cmake_minimum_required(VERSION 3.16)

project(snake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(snake src/main.cpp)

target_include_directories(snake PRIVATE /usr/local/include)

target_link_directories(snake PRIVATE /usr/local/lib)

target_link_libraries(snake raylib m pthread dl X11)