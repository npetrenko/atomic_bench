project(lockbench)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

find_package(benchmark REQUIRED)

add_executable(bench bench.cpp copy_shared.cpp)
target_link_libraries(bench benchmark pthread)

#target_compile_options(bench PRIVATE --with-libstdcxx-lock-policy=auto)
