#if 1

#include <iostream>
#include <numeric>
#include <tuple>

template<class T, std::size_t N>
struct array_iota {
    constexpr array_iota() : arr() {
        for (auto i = 0; i != N; ++i) arr[i] = i;
    }
    constexpr auto get() { return arr; }
    std::array<T, N> arr;
};

// template<class T, class Tuple>
// constexpr std::size_t type_index() {
//     constexpr Tuple t{};
//     constexpr auto size = std::tuple_size_v<Tuple>;
//     for (std::size_t i = 0; i < size; ++i) {
//         if (std::is_same_v<T, std::tuple_element_t<i, Tuple>>) {
//             return i;
//         }
//     }
//     return 0;
// }

template<class T, std::size_t N>
constexpr auto make_array_iota() {
    std::array<T, N> arr;
    for (auto i = 0; i != N; ++i) {
        arr[i] = i;
    }
    return arr;
}

int main() {
    constexpr auto arr = make_array_iota<std::size_t, 4>();
    (if (true) return 0;), (std::string s);
    return arr[3];
    // constexpr auto index = type_index<int, std::tuple<int, double>>();
    // return index;
}

#else

#include <array>
#include <iostream>
#include <numeric>

int main() {
    std::cout << std::endl;

    constexpr std::array myArray{1, 2, 3, 4, 5};
    constexpr auto sum = std::accumulate(myArray.begin(), myArray.end(), 0);
    std::cout << "sum: " << sum << std::endl;

    constexpr auto product = std::accumulate(myArray.begin(), myArray.end(), 1, std::multiplies<int>());
    std::cout << "product: " << product << std::endl;

    auto product2 = std::accumulate(myArray.begin(), myArray.end(), 1, [](auto a, auto b) { return std::string("adwd"); });
    std::cout << "product2: " << product2 << std::endl;

    std::cout << std::endl;
}

#endif