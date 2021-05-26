#include <gtest/gtest.h>

#include <type_traits>
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>

#include "../NamedType/include/NamedType/named_type.hpp"

template<class T>
class ptype;

template<class T = float>
using OutImage = fluent::NamedType<T, struct OutImageTag>;

template<class T = void>
struct static_optional {
    static_optional() {}

    template<class U = T, std::enable_if_t<!std::is_same_v<void, U>, int> = 0>
    static_optional(U t) {}
    // static_optional(T t) {}
};
// template<class T>
// static_optional(T t) -> static_optional<T>;

// static_optional() -> static_optional<void>;
namespace bda {
class empty_arg_t {};

template<class T>
inline constexpr bool is_empty_arg_v = std::is_same_v<T, empty_arg_t>;

template<class T>
inline constexpr void assert_output_arg_helper() {
    if constexpr (!bda::is_empty_arg_v<std::decay_t<T>>) {
        static_assert(std::is_lvalue_reference_v<T> && !std::is_const_v<std::remove_reference_t<T>>, "output argument is not mutable lvalue reference");
    }
}
template<class... T>
inline constexpr void assert_output_arg() { 
    (assert_output_arg_helper<T>(), ...);
}

}  // namespace bda

template<class OutTensor = bda::empty_arg_t, class OutValue = bda::empty_arg_t>
void compute(OutTensor&& aOutTensor, OutValue&& aOutValue) {
    bda::assert_output_arg<OutTensor, OutValue>();

    if constexpr (!bda::is_empty_arg_v<OutTensor>) {
        aOutTensor = xt::arange(81).reshape({9, 9});
    }
    if constexpr (!bda::is_empty_arg_v<OutValue>) {
        aOutValue = 9;
    }
}

template<class T>
void test_ref_1(T&& t) {
    bda::assert_output_arg<T>();
    std::cout << "bda::is_empty_arg_v<T> : " << bda::is_empty_arg_v<T> << "\n";
    ptype<T>();
}
template<class T = bda::empty_arg_t>
void test_ref(T&& t) {
    bda::assert_output_arg<T>();
    test_ref_1(std::forward<T>(t));
}

TEST(main, main) {
    xt::xarray<float> vArray;
    double vValue;

    compute(vArray, vValue);
    compute({}, {});
    // compute({}, 6);

    std::cout << "vArray : \n" << vArray << "\n";
    std::cout << "vValue : \n" << vValue << "\n";

    std::cout << "std::is_floating_point_v<float&> : " << std::is_floating_point_v<float&> << "\n";
    // test_ref(vValue);
    // test_ref(std::as_const(vValue));
    // test_ref(std::move(vValue));
    // test_ref(5);
    // test_ref({});
    // test(static_optional{x});
    // static_optional<void> v;
}
