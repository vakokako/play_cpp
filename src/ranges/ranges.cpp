#include <fmt/format.h>
#include <fmt/ranges.h>

#include <iostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/all.hpp>
#include <range/v3/view.hpp>
// #include <range/v3/view/filter.hpp>
#include <vector>

int main()
{
    std::cout << "variant 1:\n";
    {
        auto v = std::vector{1, 2, 3, 4, 5, 6};
        auto res_view = v | ranges::views::transform([](int i) {
                            std::cout << i << "\n";
                            return i * 2;
                        })
                        | ranges::views::filter([](int i) { return i % 3 == 0; });

        for (auto el : res_view) {
            std::cout << "el : " << el << "\n";
        }
        std::cout << "break2\n";
        const std::vector<int> res_vec = res_view | ranges::to_vector;
    }
    std::cout << "variant 2:\n";
    {
        auto v = std::vector{1, 2, 3, 4, 5, 6};

        const std::vector<int> res_view = v | ranges::views::transform([](int i) {
                                         std::cout << i << "\n";
                                         return i * 2;
                                     })
                                     | ranges::views::filter([](int i) { return i % 3 == 0; }) | ranges::to_vector;

        for (auto el : res_view) {
            std::cout << "el : " << el << "\n";
        }
        // fmt::print("v: {}\n", v);
        // fmt::print("res: {}\n", res);
    }
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
