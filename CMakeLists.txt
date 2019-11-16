project(lockbench)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

find_package(benchmark REQUIRED)

add_executable(bench src/bench.cpp src/copy_shared.cpp)
target_include_directories(bench PUBLIC src)
target_link_libraries(bench benchmark pthread)
