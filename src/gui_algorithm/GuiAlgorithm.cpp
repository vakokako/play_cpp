// #include "GuiQt.hpp"
#include <QApplication>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <tuple>

template <auto... T>
class ptype;

template <class T>
struct Property {
    using ValueT = T;
    // explicit Property(std::string aName) : name(std::move(aName)) {}
    const std::string name;
    T value;
};

template <class T>
auto* toWidget(const Property<T>& aProperty)
{
    if constexpr (std::is_same_v<T, int>) {
        auto* vSpinBox = new QSpinBox();
        vSpinBox->setValue(aProperty.value);

        auto* vWidget = new QWidget();
        vWidget->setLayout(new QHBoxLayout());
        vWidget->layout()->addWidget(
            new QLabel(QString::fromStdString(aProperty.name)));
        vWidget->layout()->addWidget(vSpinBox);
        return vWidget;
    } else {
        return new QWidget();
    }
}

template <class Alg, class = void>
struct has_properties : std::false_type {
};
template <class Alg>
struct has_properties<Alg,
                      std::void_t<decltype(std::declval<Alg>().properties())>>
    : std::true_type {
};
template <class Alg>
constexpr bool has_properties_v = has_properties<Alg>::value;

template <class Alg, class = void>
struct has_algorithms : std::false_type {
};
template <class Alg>
struct has_algorithms<Alg,
                      std::void_t<decltype(std::declval<Alg>().algorithms())>>
    : std::true_type {
};
template <class Alg>
constexpr bool has_algorithms_v = has_algorithms<Alg>::value;

template <class Derived>
struct Algorithm {
    Derived& derived_cast() { return static_cast<Derived&>(*this); }
    const Derived& derived_cast() const
    {
        return static_cast<const Derived&>(*this);
    }

    const auto& name() const { return derived_cast()._name; }
};

template <class Alg>
auto* toWidget(Algorithm<Alg>& aAlg)
{
    auto& vAlgDerived = aAlg.derived_cast();
    auto* vWidget = new QWidget();
    vWidget->setObjectName(QString::fromStdString(vAlgDerived.name()));
    vWidget->setStyleSheet(
        QString::fromStdString("QWidget#" + vAlgDerived.name()
                               + "{border:1px solid rgb(255, 0, 0);}"));

    vWidget->setLayout(new QVBoxLayout());
    vWidget->layout()->addWidget(
        new QLabel(QString::fromStdString(vAlgDerived.name())));

    if constexpr (has_properties_v<Alg>) {
        std::apply(
            [&](auto&&... aProperty) {
                ((vWidget->layout()->addWidget(toWidget(aProperty))), ...);
            },
            vAlgDerived.properties());
    }
    if constexpr (has_algorithms_v<Alg>) {
        std::apply(
            [&](auto&&... aAlgorithm) {
                ((vWidget->layout()->addWidget(toWidget(aAlgorithm))), ...);
            },
            vAlgDerived.algorithms());
    }
    return vWidget;
}

struct Alg1 : Algorithm<Alg1> {
    friend Algorithm<Alg1>;
    Property<int> size{"size", 0};
    auto properties() { return std::tie(size); }

   private:
    const std::string _name{"Alg1"};
};

struct Alg2 : Algorithm<Alg2> {
    friend Algorithm<Alg2>;
    Property<int> count{"count", 0};
    auto properties() { return std::tie(count); }

   private:
    const std::string _name{"Alg2"};
};

struct Alg3 : Algorithm<Alg3> {
    friend Algorithm<Alg3>;
    Property<int> size{"size", 0};
    Property<int> count{"count", 0};
    auto properties() { return std::tie(size, count); }

    Alg1 alg1;
    Alg2 alg2;
    auto algorithms() { return std::tie(alg1, alg2); }

   private:
    const std::string _name{"Alg3"};
};

template <class Tuple1, class Tuple2, class F, std::size_t... I>
void for_each2_impl(Tuple1&& t1, Tuple2&& t2, F&& f, std::index_sequence<I...>)
{
    (void)std::initializer_list<int>{
        (std::forward<F>(f)(std::get<I>(std::forward<Tuple1>(t1)),
                            std::get<I>(std::forward<Tuple2>(t2))),
         0)...};
}

template <class Tuple1, class Tuple2, class F>
constexpr void for_each2(Tuple1&& t1, Tuple2&& t2, F&& f)
{
    for_each2_impl(
        std::forward<Tuple1>(t1), std::forward<Tuple2>(t2), std::forward<F>(f),
        std::make_index_sequence<
            std::tuple_size<std::remove_reference_t<Tuple1>>::value>{});
}

int main(int argc, char* argv[])
{
    auto tuple1 = std::make_tuple(1, std::string{"two"});
    auto tuple2 = std::make_tuple(std::string{"one"}, 2);

    for_each2(tuple1, tuple2,
              [](auto&& i, auto&& s) { std::cout << i << '-' << s << '\n'; });

    QApplication app(argc, argv);
    QWidget dialog;
    dialog.setLayout(new QVBoxLayout());

    Alg3 alg;
    std::apply(
        [&](auto&&... aProperty) {
            ((std::cout << aProperty.name << "\n"), ...);
        },
        alg.properties());
    dialog.layout()->addWidget(toWidget(alg));

    dialog.show();

    return QApplication::exec();  // this runs the main event loop and sees to
                                  // it that cleanup is done
}
