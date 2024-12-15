cmake_minimum_required(VERSION 3.5.0)
project(key_cache VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -O0)
endif()

add_executable(key_cache main.cpp key_cache.hpp)