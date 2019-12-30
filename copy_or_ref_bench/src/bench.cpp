#include <benchmark/benchmark.h>

#include <vector>
#include <stdint.h>
#include "include.hpp"

template <class FatType>
uint64_t RunByRef(const std::vector<FatType>& vec) {
    uint64_t result = 0;
    for (const auto& elem : vec) {
        result += ByRef(elem);
    }
    return result;
}

template <class FatType>
uint64_t RunByValue(const std::vector<FatType>& vec) {
    uint64_t result = 0;
    for (auto elem : vec) {
        result += ByValue(elem);
    }
    return result;
}

template <class FatType, class Executor>
void RunBenchmark(benchmark::State& state) {
    std::vector<FatType> vec(1024, FatType{});
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(Executor{}(vec));
    }
}

BENCHMARK_TEMPLATE(RunBenchmark, Skinny, ByValueExecutor);
BENCHMARK_TEMPLATE(RunBenchmark, Skinny, ByRefExecutor);

BENCHMARK_TEMPLATE(RunBenchmark, Fat, ByValueExecutor);
BENCHMARK_TEMPLATE(RunBenchmark, Fat, ByRefExecutor);

BENCHMARK_TEMPLATE(RunBenchmark, VeryFat, ByValueExecutor);
BENCHMARK_TEMPLATE(RunBenchmark, VeryFat, ByRefExecutor);

BENCHMARK_MAIN();
