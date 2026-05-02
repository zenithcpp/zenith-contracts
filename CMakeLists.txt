cmake_minimum_required(VERSION 3.28)
project(zenith_contracts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
    -fconstexpr-depth=2147483647
    -fconstexpr-loop-limit=2147483647
    -Wall -Wextra -Werror
    -march=native
    -O3
)

  set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
add_executable(contracts src/main.cpp)

target_include_directories(contracts PRIVATE src)
