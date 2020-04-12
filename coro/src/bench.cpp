#include "inline.hpp"
#include "noinline.hpp"

#include <benchmark/benchmark.h>

template <class Range>
void RunBenchmark(benchmark::State& state) {
    Range range;
    int first = 1;
    int last = state.range(0);

    while (state.KeepRunning()) {
        auto r = range.Iterate(first, last);
        benchmark::DoNotOptimize(ReduceSum(std::move(r)));
    }

    auto test = ReduceSum(range.Iterate(1, 5));
    if (test != 10) {
        std::terminate();
    }
}

BENCHMARK_TEMPLATE(RunBenchmark, InlineSequence)->Range(4, 2 << 10);
BENCHMARK_TEMPLATE(RunBenchmark, InlineIRange)->Range(4, 2 << 10);
BENCHMARK_TEMPLATE(RunBenchmark, InlineSequenceOverRange)->Range(4, 2 << 10);
BENCHMARK_TEMPLATE(RunBenchmark, Sequence)->Range(4, 2 << 10);
BENCHMARK_TEMPLATE(RunBenchmark, IRange)->Range(4, 2 << 10);
BENCHMARK_TEMPLATE(RunBenchmark, SequenceOverRange)->Range(4, 2 << 10);

BENCHMARK_MAIN();
