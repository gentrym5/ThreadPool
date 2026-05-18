cmake_minimum_required(VERSION 3.14)
project(thread_pool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(thread_pool_demo main.cpp)
target_include_directories(thread_pool_demo PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(thread_pool_demo PRIVATE Threads::Threads)

if(MSVC)
    target_compile_options(thread_pool_demo PRIVATE /W4 /permissive-)
else()
    target_compile_options(thread_pool_demo PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()
