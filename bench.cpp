#include <benchmark/benchmark.h>

#include <memory>

std::shared_ptr<int> getCopy();

void CopyShared(benchmark::State& state) {
    while (state.KeepRunning()) {
	auto cp = getCopy();
	benchmark::DoNotOptimize(cp);
    }
}

void LockedAdd(benchmark::State& state) {
    unsigned int val;
    while (state.KeepRunning()) {
	asm volatile("lock addl $1, %[val]" : [val] "=m"(val));
	benchmark::DoNotOptimize(val);
    }
}

void SimpleAdd(benchmark::State& state) {
    unsigned int val;
    while (state.KeepRunning()) {
	asm volatile("addl $1, %[val]" : [val] "=m"(val));
	benchmark::DoNotOptimize(val);
    }
}

BENCHMARK(LockedAdd);
BENCHMARK(SimpleAdd);
BENCHMARK(CopyShared);

BENCHMARK_MAIN();
