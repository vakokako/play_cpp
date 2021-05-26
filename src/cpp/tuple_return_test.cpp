#include <gtest/gtest.h>

#include <xtensor/xtensor.hpp>
#include <type_traits>

#include "../NamedType/include/NamedType/named_type.hpp"

template<class T>
void ptype(T&& t);
template<class T>
void ptype();

/**************************************************************************************************************/

/* is_specialization_of */

template<class T, template<class...> class TT>
struct is_specialization_of : std::false_type {};
template<template<class...> class TT, class... Ts>
struct is_specialization_of<TT<Ts...>, TT> : std::true_type {};
template<class T, template<class...> class TT>
inline constexpr bool is_specialization_of_v = is_specialization_of<T, TT>::value;

/* has_type */

template<class T, class... U>
struct has_type_helper : std::disjunction<std::is_same<T, U>...> {};
template<class T, class... U>
struct has_type_helper<T, std::tuple<U...>> : has_type_helper<T, U...> {};

template<template<class...>class T, class... U>
struct has_template_type_helper : std::disjunction<is_specialization_of<U, T>...> {};
template<template<class...>class T, class... U>
struct has_template_type_helper<T, std::tuple<U...>> : has_template_type_helper<T, U...> {};

template<class T, class... U>
constexpr bool has_type() {
    return has_type_helper<T, U...>::value;
}
template<template<class...> class T, class... U>
constexpr bool has_type() {
    return has_template_type_helper<T, U...>::value;
}

/* tuple_element_index */

template<class T, class Tuple>
struct tuple_element_index_helper;

template<class T>
struct tuple_element_index_helper<T, std::tuple<>> {
    static constexpr std::size_t value = 0;
};

template<class T, class... Rest>
struct tuple_element_index_helper<T, std::tuple<T, Rest...>> {
    static constexpr std::size_t value = 0;
    using RestTuple                    = std::tuple<Rest...>;
    static_assert(tuple_element_index_helper<T, RestTuple>::value == std::tuple_size_v<RestTuple>,
                  "type appears more than once in tuple");
};

template<class T, class First, class... Rest>
struct tuple_element_index_helper<T, std::tuple<First, Rest...>> {
    using RestTuple                    = std::tuple<Rest...>;
    static constexpr std::size_t value = 1 + tuple_element_index_helper<T, RestTuple>::value;
};

template<class T, class Tuple>
struct tuple_element_index {
    static constexpr std::size_t value = tuple_element_index_helper<T, Tuple>::value;
    static_assert(value < std::tuple_size_v<Tuple>, "type does not appear in tuple");
};

template<class T, class Tuple>
inline constexpr std::size_t tuple_element_index_v = tuple_element_index<T, Tuple>::value;

/* tuple_template_element_index */

template<template<class...>class T, class Tuple, class = void>
struct tuple_template_element_index_helper;

template<template<class...>class T>
struct tuple_template_element_index_helper<T, std::tuple<>> {
    static constexpr std::size_t value = 0;
};

template<template<class...>class T, class First, class... Rest>
struct tuple_template_element_index_helper<T, std::tuple<First, Rest...>, std::enable_if_t<is_specialization_of_v<First, T>>> {
    static constexpr std::size_t value = 0;
    using RestTuple                    = std::tuple<Rest...>;
    static_assert(tuple_template_element_index_helper<T, RestTuple>::value == std::tuple_size_v<RestTuple>,
                  "template type appears more than once in tuple");
};

template<template<class...>class T, class First, class... Rest>
struct tuple_template_element_index_helper<T, std::tuple<First, Rest...>, std::enable_if_t<!is_specialization_of_v<First, T>>> {
    using RestTuple                    = std::tuple<Rest...>;
    static constexpr std::size_t value = 1 + tuple_template_element_index_helper<T, RestTuple>::value;
};

template<template<class...>class T, class Tuple>
struct tuple_template_element_index {
    static constexpr std::size_t value = tuple_template_element_index_helper<T, Tuple>::value;
    static_assert(value < std::tuple_size_v<Tuple>, "template type does not appear in tuple");
};

template<template<class...>class T, class Tuple>
inline constexpr std::size_t tuple_template_element_index_v = tuple_template_element_index<T, Tuple>::value;

/* get_by_type */

template<class EType, class Tuple, std::enable_if_t<is_specialization_of_v<std::decay_t<Tuple>, std::tuple>, int> = 0>
inline constexpr auto&& get_by_type(Tuple&& aTuple) {
    return std::get<tuple_element_index_v<EType, std::decay_t<Tuple>>>(std::forward<Tuple>(aTuple));
}
template<class EType, class Type, std::enable_if_t<std::is_same_v<std::decay_t<Type>, EType>, int> = 0>
inline constexpr auto&& get_by_type(Type&& aObject) {
    return std::forward<Type>(aObject);
}

template<template<class...>class EType, class Tuple, std::enable_if_t<is_specialization_of_v<std::decay_t<Tuple>, std::tuple>, int> = 0>
inline constexpr auto&& get_by_type(Tuple&& aTuple) {
    return std::get<tuple_template_element_index_v<EType, std::decay_t<Tuple>>>(std::forward<Tuple>(aTuple));
}
template<template<class...>class EType, class Type, std::enable_if_t<is_specialization_of_v<std::decay_t<Type>, EType>, int> = 0>
inline constexpr auto&& get_by_type(Type&& aObject) {
    return std::forward<Type>(aObject);
}

/**************************************************************************************************************/

template<class T>
class TemplateType{};

TEST(tuple_return_type, is_specialization_of) {
    static_assert(is_specialization_of_v<TemplateType<float>, TemplateType>);
    static_assert(is_specialization_of_v<TemplateType<float>, TemplateType>);
}

TEST(tuple_return_type, get_by_type) {
    std::tuple<TemplateType<int>, float> vTuple;
    static_assert(std::is_same_v<TemplateType<int>&, decltype(get_by_type<TemplateType>(vTuple))>);

    TemplateType<int> vObject;
    auto& vObjectRef = vObject;
    const auto& vObjectConstRef = vObject;
    static_assert(std::is_same_v<TemplateType<int>&, decltype(get_by_type<TemplateType>(vObject))>);
    static_assert(std::is_same_v<TemplateType<int>const&, decltype(get_by_type<TemplateType>(vObjectConstRef))>);
}


template<class T>
struct Image : public fluent::NamedType<T, struct ImageTag> {
    using fluent::NamedType<T, struct ImageTag>::NamedType;
};
template<class T>
struct Name : public fluent::NamedType<T, struct NameTag> {
    using fluent::NamedType<T, struct NameTag>::NamedType;
};
template<class T>
struct Surname : public fluent::NamedType<T, struct SurnameTag> {
    using fluent::NamedType<T, struct SurnameTag>::NamedType;
};

template<class T>
struct OutRef : public fluent::NamedType<T&, struct OutRefTag> {
    using fluent::NamedType<T&, struct OutRefTag>::NamedType;
};
template<class T>
OutRef(T t) -> OutRef<T>;


template<class U = std::tuple<>>
void func(U&& aOut) {
    if constexpr (has_type<OutRef, std::decay_t<U>>()) {
        auto&& vImage = get_by_type<OutRef>(aOut).get();
        vImage = xt::arange(81).reshape({9, 9});
    }

    if constexpr (has_type<Name, std::decay_t<U>>()) {
        auto&& vName = get_by_type<Name>(aOut);
        vName.get() = "name is pidor";
    }

    if constexpr (has_type<Surname, std::decay_t<U>>()) {
        auto&& vName = get_by_type<Surname>(aOut);
        vName.get() = "surname is pidor";
    }

    if constexpr (has_type<double, std::decay_t<U>>()) {
        auto&& vAge = get_by_type<double>(aOut);
        vAge = 69;
    }
}

TEST(tuple_return_type, function_run) {
    xt::xtensor<float, 2> vTensor;
    std::tuple vResult{OutRef{vTensor}};

    // static_assert(is_specialization_of<decltype(OutRef{vTensor}), Name>::value);
    // static_assert(has_type<Name, decltype(vResult)>());

    func(vResult);
    func({});
    
    // OutRef vOut{vTensor};
    // // ptype(vOut);

    // std::tuple t{};



    // std::cout << "name: " << get_by_type<Name>(vResult).get() << "\n";
    // std::cout << "surname: " << get_by_type<Surname>(vResult).get() << "\n";
    // std::cout << "age: " << get_by_type<double>(vResult) << "\n";
}



TEST(tuple_return_type, static_checks) {
    static_assert(tuple_template_element_index_v<Name, std::tuple<Name<double>, int>> == 0);
    static_assert(tuple_template_element_index_v<Name, std::tuple<double, Name<int>>> == 1);
    static_assert(!has_type<Name, double, int>());
    static_assert(has_type<Name, Name<double>, int>());
}



TEST(tuple_return_type, HasType) {
    static_assert(has_type<int, int>());
    static_assert(has_type<int, std::tuple<int>>());
    static_assert(!has_type<double, std::tuple<int>>());
    static_assert(has_type<TemplateType, std::tuple<TemplateType<int>>>());
    static_assert(has_type<std::vector, std::tuple<std::vector<int>>>());
    static_assert(!has_type<TemplateType, std::tuple<int>>());
    static_assert(has_type<TemplateType, TemplateType<int>>());
    static_assert(!has_type<TemplateType, int>());
}