#include <benchmark/benchmark.h>

#include <any>
#include <numeric>

namespace allocate
{
auto cartesian_product(const std::vector<std::vector<std::any>>& aInput)
{
    std::size_t vResultsSize = std::accumulate(aInput.cbegin(), aInput.cend(), 1, [](size_t res, auto&& aVec) { return res * aVec.size(); });
    std::vector<std::vector<std::any>> vResult(vResultsSize, std::vector<std::any>(aInput.size()));
    // std::for_each(vResult.begin(), vResult.end(), [&](auto&& v) { v.resize(aInput.size()); });

    for (size_t i = 0; i < vResult.size(); ++i) {
        for (size_t j = 0; j < aInput.size(); ++j) {
            size_t vIndexHelper = 1;
            for (size_t k = j + 1; k < aInput.size(); ++k) {
                vIndexHelper *= aInput[k].size();
            }
            vResult[i][j] = aInput[j][i / vIndexHelper % aInput[j].size()];
        }
    }
    return vResult;
}
}  // namespace resize

namespace resize
{
auto cartesian_product(const std::vector<std::vector<std::any>>& aInput)
{
    std::size_t vResultsSize = std::accumulate(aInput.cbegin(), aInput.cend(), 1, [](size_t res, auto&& aVec) { return res * aVec.size(); });
    std::vector<std::vector<std::any>> vResult(vResultsSize);
    std::for_each(vResult.begin(), vResult.end(), [&](auto&& v) { v.resize(aInput.size()); });

    for (size_t i = 0; i < vResult.size(); ++i) {
        for (size_t j = 0; j < aInput.size(); ++j) {
            size_t vIndexHelper = 1;
            for (size_t k = j + 1; k < aInput.size(); ++k) {
                vIndexHelper *= aInput[k].size();
            }
            vResult[i][j] = aInput[j][i / vIndexHelper % aInput[j].size()];
        }
    }
    return vResult;
}
}  // namespace resize

namespace reserve
{
auto cartesian_product(const std::vector<std::vector<std::any>>& aInput)
{
    std::size_t vResultsSize = std::accumulate(aInput.cbegin(), aInput.cend(), 1, [](size_t res, auto&& aVec) { return res * aVec.size(); });
    std::vector<std::vector<std::any>> vResult;
    vResult.reserve(vResultsSize);
    // std::for_each(vResult.begin(), vResult.end(), [&](auto&& v) { v.reserve(aInput.size()); });

    for (size_t i = 0; i < vResultsSize; ++i) {
        std::vector<std::any> vTmp;
        vTmp.reserve(aInput.size());
        for (size_t j = 0; j < aInput.size(); ++j) {
            size_t vIndexHelper = 1;
            for (size_t k = j + 1; k < aInput.size(); ++k) {
                vIndexHelper *= aInput[k].size();
            }
            vTmp.push_back(aInput[j][i / vIndexHelper % aInput[j].size()]);
        }
        vResult.push_back(vTmp);
    }
    return vResult;
}
}  // namespace reserve

auto BM_CartesianProduct_Allocate = [](benchmark::State& state, std::size_t size) {
    auto input = std::vector<std::vector<std::any>>(2, std::vector<std::any>(size, std::any{1}));

    for (auto _ : state) {
        auto result = allocate::cartesian_product(input);
        benchmark::DoNotOptimize(result.data());
    }
};

auto BM_CartesianProduct_Resize = [](benchmark::State& state, std::size_t size) {
    auto input = std::vector<std::vector<std::any>>(2, std::vector<std::any>(size, std::any{1}));

    for (auto _ : state) {
        auto result = resize::cartesian_product(input);
        benchmark::DoNotOptimize(result.data());
    }
};
auto BM_CartesianProduct_Reserve = [](benchmark::State& state, std::size_t size) {
    auto input = std::vector<std::vector<std::any>>(2, std::vector<std::any>(size, std::any{1}));

    for (auto _ : state) {
        auto result = reserve::cartesian_product(input);
        benchmark::DoNotOptimize(result.data());
    }
};

int main(int argc, char** argv)
{
    // Create targets for which you want to run
    // you can store discovered Halide::Target here
    std::vector<std::pair<std::string, int>> benchmarks;
    benchmarks.emplace_back(std::string("big"), 1000);

    // Register benchmarks
    for (auto& benchmark : benchmarks) {
        benchmark::RegisterBenchmark("resize", BM_CartesianProduct_Resize, benchmark.second);
        benchmark::RegisterBenchmark("reserve", BM_CartesianProduct_Reserve, benchmark.second);
        benchmark::RegisterBenchmark("allocate", BM_CartesianProduct_Allocate, benchmark.second);
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}