#include "hidden.hpp"

#include <benchmark/benchmark.h>

#include <functional>

void Bind(benchmark::State &state) {
    size_t counter = 0;
    const std::function binded = GetIncrementer<UseBind::T>(&counter);

    while (state.KeepRunning()) {
        binded();
        benchmark::DoNotOptimize(counter);
    }
}

void Function(benchmark::State &state) {
    size_t counter = 0;
    const std::function callback = GetIncrementer<UseBind::F>(&counter);

    while (state.KeepRunning()) {
        callback();
        benchmark::DoNotOptimize(counter);
    }
}

void NotInlined(benchmark::State &state) {
    size_t counter = 0;

    while (state.KeepRunning()) {
        NoinlineIncrement(&counter);
        benchmark::DoNotOptimize(counter);
    }
}

void Inlined(benchmark::State &state) {
    size_t counter = 0;
    while (state.KeepRunning()) {
        ++counter;
        benchmark::DoNotOptimize(counter);
    }
}

BENCHMARK(Bind);
BENCHMARK(Function);
BENCHMARK(NotInlined);
BENCHMARK(Inlined);

BENCHMARK_MAIN();
