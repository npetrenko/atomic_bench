#include <benchmark/benchmark.h>

#include <fmt/core.h>

#include <string_view>
#include <utility>
#include <sstream>

namespace {
std::string joinSstream(std::string_view left, std::string_view right) {
    std::ostringstream ss;
    ss << left << ", " << right;

    return std::move(ss).str();
}

std::string joinFormat(std::string_view left, std::string_view right) {
    return fmt::format("{}, {}", left, right);
}

template <auto Joiner>
void RunBenchmark(benchmark::State& state) {
    constexpr std::array<std::string_view, 3> words{"hello", "world", "unknown"};

    size_t pos = 0;
    while (state.KeepRunning()) {
        auto left = words[pos];
        ++pos;
        pos = pos % words.size();
        auto right = words[pos];

        benchmark::DoNotOptimize(Joiner(left, right));
    }
}
}  // namespace

BENCHMARK_TEMPLATE(RunBenchmark, joinSstream);
BENCHMARK_TEMPLATE(RunBenchmark, joinFormat);

BENCHMARK_MAIN();
