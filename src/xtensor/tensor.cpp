#include <xtensor/xview.hpp>
#include <xtensor/xtensor.hpp>

template<class...Values>
bool func(Values...values) {
    return 1==0 || (... || (1 == values));
}

int main() {
    xt::xtensor<float, 2> vTensor({5, 5});
    auto view = xt::view(vTensor, xt::range(1, 3), xt::range(1, 3));
    auto it = view.begin();
    it++;
    it++;
    it++;
    it++;
    it++;
    it = std::next(it, -1);
    std::cout << std::distance(view.begin(), it) << "\n";
    std::cout << sizeof(size_t) << " <> " << sizeof(it) << "\n";

    int i = 1;
    std::next(&i);
}