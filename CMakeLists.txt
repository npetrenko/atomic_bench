project(cpp_benchmark)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -mtune=native")

find_package(benchmark REQUIRED)

include(cmake/AddBench.cmake)

add_subdirectory(atomic_bench)
add_subdirectory(copy_or_ref_bench)
