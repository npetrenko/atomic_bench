#include <benchmark/benchmark.h>

#include <functional>

__attribute__((noinline))
void NoinlineIncrement(size_t *val) {
    *val += 1;
}

void Increment(size_t *val) {
    *val += 1;
}

void Bind(benchmark::State &state) {
    size_t counter = 0;
    const auto binded = std::bind(Increment, &counter);

    while (state.KeepRunning()) {
        binded();
        benchmark::DoNotOptimize(counter);
    }
}

void Function(benchmark::State &state) {
    size_t counter = 0;
    const auto callback = std::function<void()>{[&counter] { ++counter; }};

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
