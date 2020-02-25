project(cpp_benchmark)

cmake_minimum_required(VERSION 3.8)

set(WFLAGS "-Werror -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wenum-compare -Wnon-virtual-dtor -Woverloaded-virtual -Wredundant-decls -Wsign-promo")

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WFLAGS} -Wno-c++98-compat")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG -g -O3 -march=native -mtune=native")
set(CMAKE_CXX_FLAGS_ASAN "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")

find_package(benchmark REQUIRED)

include(cmake/AddBench.cmake)

add_subdirectory(atomic_bench)
add_subdirectory(copy_or_ref_bench)
add_subdirectory(variant_bench)
add_subdirectory(functional_bench)
