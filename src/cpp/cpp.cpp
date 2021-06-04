#include <any>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <xtensor/xtensor.hpp>

int main()
{
    auto vec = std::vector{1, 2, 3, 0, 4, 0};
    std::cout << std::accumulate(vec.cbegin(), vec.cend(), 0, [](size_t aSum, size_t aV) { return aSum + static_cast<unsigned long>(aV != 0); });

    // tensor::inner_types::
}