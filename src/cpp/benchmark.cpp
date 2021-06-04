
#include <benchmark/benchmark.h>

#include <numeric>

template <class F>
static void BenchmarkBody(benchmark::State& aState, F f)
{
    std::vector<int> v(static_cast<size_t>(aState.range(0)));

    for ([[maybe_unused]] auto _ : aState) {
        f(v);
        benchmark::DoNotOptimize(v.data());
    }
}

int main(int argc, char** argv)
{
    benchmark::Initialize(&argc, argv);

    auto call = [](auto&& v) { std::accumulate(v.cbegin(), v.cend(), 0); };
    std::array vBenchmarks{benchmark::RegisterBenchmark("hello", BenchmarkBody<decltype(call)>, call)};
    for (auto* bench : vBenchmarks) {
        bench->Arg(500000);
    }

    benchmark::RunSpecifiedBenchmarks();
}