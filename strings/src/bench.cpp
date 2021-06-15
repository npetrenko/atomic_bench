#include <benchmark/benchmark.h>

#include <optional>
#include <string_view>
#include <cassert>

namespace {
enum class Side {
    Sell,
    Buy,
};

std::optional<Side> parseSideSlow(std::string_view sideS)
{
    if (sideS == "Sell") {
        return Side::Sell;
    } else if (sideS == "Buy") {
        return Side::Buy;
    } else {
        return std::nullopt;
    }
}

std::optional<Side> parseSide(std::string_view sideS)
{
    if (sideS.empty()) {
        return std::nullopt;
    }

    switch (sideS[0]) {
        case 'S': {
            assert(sideS == "Sell");
            return Side::Sell;
        }
        case 'B': {
            assert(sideS == "Buy");
            return Side::Buy;
        }
        default: {
            return std::nullopt;
        }
    }
}

std::optional<Side> parseSideUnsafe(std::string_view sideS)
{
    switch (sideS[0]) {
        case 'S': {
            assert(sideS == "Sell");
            return Side::Sell;
        }
        case 'B': {
            assert(sideS == "Buy");
            return Side::Buy;
        }
        default: {
            return std::nullopt;
        }
    }
}

std::optional<Side> parseSideFaster(std::string_view sideS)
{
    switch (sideS.size()) {
        case 4: {
            assert(sideS == "Sell");
            return Side::Sell;
        }
        case 3: {
            assert(sideS == "Buy");
            return Side::Buy;
        }
        default: {
            return std::nullopt;
        }
    }
}
}

template <auto Parser>
void RunBenchmark(benchmark::State& state) {
    constexpr std::array sides{
        "Buy",  "Sell",    "Unknown", "Sell",    "Buy",     "Unknown", "Unknown", "Sell", "Buy",
        "Sell", "Unknown", "Sell",    "Unknown", "Unknown", "Sell",    "Buy",     "Buy",  "Buy",
    };

    size_t pos = 0;

    while (state.KeepRunning()) {
        std::optional<Side> s = Parser(sides[pos]);
        benchmark::DoNotOptimize(s);
        ++pos;
        pos = pos % sides.size();
    }
}

BENCHMARK_TEMPLATE(RunBenchmark, parseSideSlow);
BENCHMARK_TEMPLATE(RunBenchmark, parseSide);
BENCHMARK_TEMPLATE(RunBenchmark, parseSideUnsafe);
BENCHMARK_TEMPLATE(RunBenchmark, parseSideFaster);

BENCHMARK_MAIN();
