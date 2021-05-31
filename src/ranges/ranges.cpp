#include <fmt/format.h>
#include <fmt/ranges.h>

#include <iostream>
#include <range/v3/all.hpp>
#include <range/v3/view.hpp>
// #include <range/v3/view/filter.hpp>
#include <vector>

int main()
{
    auto v = std::vector{1, 2, 3, 4, 5, 6};

    const std::vector<int> res = v | ranges::views::transform([](int i) {
                                     std::cout << i << "\n";
                                     return i * 2;
                                 })
                                 | ranges::views::filter([](int i) { return i % 3 == 0; }) | ranges::to_vector;

    fmt::print("v: {}\n", v);
    fmt::print("res: {}\n", res);
}

// #include <array>
// #include <iostream>
// #include <range/v3/all.hpp>

// int main()
// {
//     std::vector<int> v {1, 2, -3, 4, -5, 6, -7, 8, 9};

//     std::vector<int> v2 = v | ranges::view::transform([](int x){ return x * 2; }) | ranges::view::filter([](int x) { return x > 3; });

//     for (auto e : v2) {
//         std::cout << " " << e;
//     }
//     std::cout << std::endl;
// }
