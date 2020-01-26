#include <benchmark/benchmark.h>

#include "my_variants.hpp"

template <class Variant, class CreationTag>
void RunBenchmark(benchmark::State& state) {
    std::mt19937 rd{1234};
    
    auto vec = InstantiationGroup<Variant>::OverloadedCreate(1024*32, &rd, CreationTag{});
    size_t pos = 0;
    auto update_pos = [&] {
        ++pos;
        if (pos == vec.size()) {
            pos = 0;
        }
    };

    size_t num_ints = 0;
    struct {
        size_t* const num_ints;

        void operator()(int) {
            ++(*num_ints);
        }
        void operator()(float) {
        }
    } visitor{&num_ints};
    
    while (state.KeepRunning()) {
        vec[pos].Accept(visitor);
        benchmark::DoNotOptimize(num_ints);
        update_pos();
    }
}

BENCHMARK_TEMPLATE(RunBenchmark, SimpleVariant, NTags::Random);
BENCHMARK_TEMPLATE(RunBenchmark, StlVariant, NTags::Random);
BENCHMARK_TEMPLATE(RunBenchmark, UnionVariant, NTags::Random);

BENCHMARK_TEMPLATE(RunBenchmark, SimpleVariant, NTags::Ints);
BENCHMARK_TEMPLATE(RunBenchmark, StlVariant, NTags::Ints);
BENCHMARK_TEMPLATE(RunBenchmark, UnionVariant, NTags::Ints);

BENCHMARK_TEMPLATE(RunBenchmark, SimpleVariant, NTags::Floats);
BENCHMARK_TEMPLATE(RunBenchmark, StlVariant, NTags::Floats);
BENCHMARK_TEMPLATE(RunBenchmark, UnionVariant, NTags::Floats);

BENCHMARK_MAIN();
