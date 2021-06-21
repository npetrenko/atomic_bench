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

std::string incorrectJoinPlusLeft(std::string_view left, std::string_view right) {
    return std::string(left) + right.data();
}

std::string incorrectJoinPlusRight(std::string_view left, std::string_view right) {
    return left.data() + std::string(right);
}

std::string joinFast(std::string_view left, std::string_view right) {
    std::string res;
    res.resize(left.size() + right.size() + 2);

    std::copy(left.begin(), left.end(), res.begin());

    res.push_back(',');
    res.push_back(' ');

    std::copy(right.begin(), right.end(), res.begin());
    return res;
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
BENCHMARK_TEMPLATE(RunBenchmark, joinFast);

BENCHMARK_TEMPLATE(RunBenchmark, incorrectJoinPlusLeft);
BENCHMARK_TEMPLATE(RunBenchmark, incorrectJoinPlusRight);

BENCHMARK_MAIN();
