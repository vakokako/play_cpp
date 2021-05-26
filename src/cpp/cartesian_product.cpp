#include <any>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <numeric>

template <class T>
struct Property {
    std::string name;
    T value;
};

struct Alg {
    struct {
        Property<int> mRadius{"radius"};
    } properties;
};

namespace bda
{
template <class T>
struct remove_cvref : std::remove_cv<std::remove_reference_t<T>> {
};
template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <class T>
struct tuple_remove_cvref;
template <class... T>
struct tuple_remove_cvref<std::tuple<T...>> {
    using type = std::tuple<bda::remove_cvref_t<T>...>;
};
template <class T>
using tuple_remove_cvref_t = typename tuple_remove_cvref<T>::type;
}  // namespace bda

namespace tuple
{
namespace detail
{
template <class T>
constexpr std::size_t tuple_size_v = std::tuple_size_v<bda::remove_cvref_t<T>>;

template <std::size_t I, class T, std::size_t... N>
static constexpr auto index_helper(std::index_sequence<N...>) noexcept
{
    return (1 * ... * (I < N ? tuple_size_v<std::tuple_element_t<N, T>> : 1));
}
template <std::size_t N, std::size_t I, class T>
static constexpr auto index() noexcept
{
    return N / index_helper<I, T>(std::make_index_sequence<tuple_size_v<T>>()) % tuple_size_v<std::tuple_element_t<I, T>>;
}

template <std::size_t N, class T, std::size_t... I>
static constexpr auto cartesian_product(T t, std::index_sequence<I...>) noexcept
{
    return std::forward_as_tuple(std::get<index<N, I, T>()>(std::get<I>(t))...);
}
template <class T, std::size_t... N>
static constexpr auto cartesian_product(T t, std::index_sequence<N...>) noexcept
{
    return std::make_tuple(cartesian_product<N>(t, std::make_index_sequence<tuple_size_v<T>>())...);
}

template <class T>
auto emplace_tuple_no_ref(T t)
{
    return std::apply([](auto&&... x) { return std::make_tuple(x...); }, t);
}
template <class T>
auto tuple2_no_ref(T t)
{
    return std::apply([](auto&&... x) { return std::make_tuple(emplace_tuple_no_ref(x)...); }, t);
}
}  // namespace detail

template <class... T>
constexpr auto cartesian_product_ref(T&&... t) noexcept
{
    constexpr auto N = sizeof...(T) ? (1 * ... * detail::tuple_size_v<T>) : 0;
    return detail::cartesian_product(std::forward_as_tuple(t...), std::make_index_sequence<N>());
}

template <class... T>
constexpr auto cartesian_product(T&&... t) noexcept(noexcept(decltype(detail::tuple2_no_ref(cartesian_product_ref(t...)))(cartesian_product_ref(t...))))
{
    auto r = cartesian_product_ref(t...);
    using R = decltype(detail::tuple2_no_ref(r));
    // return R(r);
    return detail::tuple2_no_ref(r);
}

}  // namespace tuple

namespace vector
{
namespace detail
{
template <class T>
constexpr std::size_t tuple_size_v = std::tuple_size_v<bda::remove_cvref_t<T>>;

template <std::size_t I, class T, std::size_t... N>
static constexpr auto index_helper(T&& t, const std::index_sequence<N...>&) noexcept
{
    return (1 * ... * (I < N ? std::get<N>(t).size() : 1));
}
template <std::size_t I, class T, class V>
static constexpr auto index(std::size_t N, V&& v, T&& t) noexcept
{
    return N / index_helper<I>(t, std::make_index_sequence<tuple_size_v<T>>()) % v.size();  // tuple_size_v<std::tuple_element_t<I, T>>;
}

template <class T, std::size_t... I>
static constexpr auto cartesian_product(size_t N, T&& t, const std::index_sequence<I...>&) noexcept
{
    return std::forward_as_tuple(std::get<I>(t)[index<I>(N, std::get<I>(t), t)]...);
}
template <class T>
static constexpr auto cartesian_product(T&& t, std::size_t N) noexcept
{
    using Combination = bda::remove_cvref_t<decltype(cartesian_product(0, t, std::make_index_sequence<tuple_size_v<T>>()))>;
    std::vector<Combination> result;
    result.reserve(N);
    auto index_sequence = std::make_index_sequence<tuple_size_v<T>>();
    for (std::size_t i = 0; i < N; ++i) {
        result.push_back(cartesian_product(i, t, index_sequence));
    }
    return result;
}
template <class V, class T>
auto emplace_tuple_no_ref(V&& v, T&& t)
{
    std::apply([&](auto&&... x) { v.emplace_back(x...); }, t);
}
template <class T>
auto tuple2_no_ref(T&& t)
{
    using new_type = bda::tuple_remove_cvref_t<bda::remove_cvref_t<decltype(t[0])>>;
    std::vector<new_type> res;
    res.reserve(t.size());
    for (auto&& tuple_ref : t) {
        emplace_tuple_no_ref(res, tuple_ref);
    }
    return res;
}
}  // namespace detail

template <class... T>
constexpr auto cartesian_product_ref(T&&... t) noexcept
{
    auto N = sizeof...(T) ? (1 * ... * t.size()) : 0;
    return detail::cartesian_product(std::forward_as_tuple(t...), N);
}

template <class... T>
constexpr auto cartesian_product(T&&... t) noexcept(noexcept(detail::tuple2_no_ref(cartesian_product_ref(t...))))
{
    auto r = cartesian_product_ref(t...);
    return detail::tuple2_no_ref(r);
}

}  // namespace vector

std::ostream& operator<<(std::ostream& out, const std::vector<std::any>& vec)
{
    for (auto&& v : vec) {
        out << std::any_cast<int>(v) << " ";
    }
    return out;
}
template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec)
{
    for (auto&& v : vec) {
        out << v << " ";
    }
    return out;
}
namespace any
{
auto cartesian_product(const std::vector<std::vector<std::any>>& aInput)
{
    std::size_t vResultsSize = std::accumulate(aInput.cbegin(), aInput.cend(), 1, [](size_t res, auto&& aVec) { return res * aVec.size(); });
    std::vector<std::vector<std::any>> vResult(vResultsSize, std::vector<std::any>(aInput.size()));

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

}  // namespace any

class Catch
{
   public:
    int _v;
    Catch(int v) : _v(v) { std::cout << "Catch(" << _v << ")\n"; }
    Catch(const Catch& c) : _v(c._v) { std::cout << "Catch(const& " << _v << ")\n"; }
    Catch(Catch&& c) : _v(c._v) { std::cout << "Catch(&& " << _v << ")\n"; }
    Catch& operator=(const Catch& c)
    {
        _v = c._v;
        std::cout << "Catch =(const& " << _v << ")\n";
        return *this;
    }
    Catch& operator=(Catch&& c)
    {
        _v = c._v;
        std::cout << "Catch =(&& " << _v << ")\n";
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out, const Catch& c) { return out << "Catch(" << c._v << ")"; }
};

int main()
{
    using namespace std::string_literals;
    auto print_tuple = [](auto&& tuple) {
        std::cout << "tuple : ";
        std::apply([](auto&&... v) { ((std::cout << v << " "), ...); }, tuple);
        std::cout << "\n";
    };

#if 0
    auto vec1 = std::vector{1, 2, 3, 4};
    auto vec2 = std::vector{Catch(1)};
    std::cout << "vector::cartesian_product\n";
    auto vec_res = vector::cartesian_product(vec1, vec2);
    std::cout << "vector::cartesian_product end\n";
    for (auto&& t : vec_res) {
        print_tuple(t);
    }

#elif 1
    auto vec_any1 = std::vector<std::any>{1, 2, 3};
    auto vec_any2 = std::vector<std::any>{1, 2, 3};
    auto res = any::cartesian_product(std::vector{vec_any1, vec_any2});
    for (auto&& combination : res) {
        std::cout << "vec any : " << combination << "\n";
    }
#else

    auto res = tuple::cartesian_product(std::tuple{1, 2, 3}, std::tuple{4, 5, 6}, std::tuple{"a"s, "b"s, "c"s});

    std::apply([&](auto&&... combination) { (print_tuple(combination), ...); }, res);
#endif
}