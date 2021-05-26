#include <gtest/gtest.h>

template<typename T>
struct XtensorTest : public testing::Test {
    using NewType = T;
};

using TestTypes = testing::Types<float, int>;
TYPED_TEST_SUITE(XtensorTest, TestTypes);

TYPED_TEST(XtensorTest, containers) {
    if constexpr (std::is_same_v<TypeParam, int>) {
        std::cout << "int\n";
    } else if constexpr (std::is_same_v<TypeParam, float>) {
        std::cout << "float\n";
    }
    throw std::runtime_error("hello");
}

namespace bda {
enum class TensorLayout { INVALID, ROW_MAJOR, COLUMN_MAJOR, ROW_AND_COLUMN_MAJOR };

template<class Strides>
inline TensorLayout layoutFromStrides(Strides&& aStrides) {
    if (!aStrides.size() || aStrides.size() == 1) {
        return TensorLayout::ROW_AND_COLUMN_MAJOR;
    }

    auto sign = [](auto v) -> int {
        using T = decltype(v);
        return (T(0) < v) - (v < T(0));
    };
    constexpr int InvalidSign = std::numeric_limits<int>::min();

    int vDiffSign     = 0;
    auto vLastNonZero = *std::begin(aStrides);
    for (auto aStride : aStrides) {
        if (aStride == 0) {
            continue;
        }
        if (vLastNonZero == 0) {
            vLastNonZero = aStride;
            continue;
        }

        int vCurr = 0;
        if constexpr (std::is_signed_v<std::decay_t<decltype(aStride)>>) {
            vCurr = sign(std::abs(aStride) - std::abs(vLastNonZero));
        } else {
            vCurr = sign(static_cast<std::ptrdiff_t>(aStride) - static_cast<std::ptrdiff_t>(vLastNonZero));
        }
        vLastNonZero = aStride;

        if (vDiffSign == vCurr) {
            continue;
        }
        if (vDiffSign == 0) {
            vDiffSign = vCurr;
            continue;
        }

        vDiffSign = InvalidSign;
        break;
    };

    switch (vDiffSign) {
        case -1:
            return TensorLayout::ROW_MAJOR;
        case 0:
            return TensorLayout::ROW_AND_COLUMN_MAJOR;
        case 1:
            return TensorLayout::COLUMN_MAJOR;
        case InvalidSign:
        default:
            return TensorLayout::INVALID;
    }
}
}  // namespace bda

TEST(TensorShape, LayoutFromStrides) {
    EXPECT_EQ(bda::layoutFromStrides(std::vector<std::size_t>{}), bda::TensorLayout::ROW_AND_COLUMN_MAJOR);
    EXPECT_EQ(bda::layoutFromStrides(std::array{2}), bda::TensorLayout::ROW_AND_COLUMN_MAJOR);
    EXPECT_EQ(bda::layoutFromStrides(std::array{1, 2, 4, 8}), bda::TensorLayout::COLUMN_MAJOR);
    EXPECT_EQ(bda::layoutFromStrides(std::array{8, 4, 2, 1}), bda::TensorLayout::ROW_MAJOR);
    EXPECT_EQ(bda::layoutFromStrides(std::array{8, 4, -2, 1}), bda::TensorLayout::ROW_MAJOR);
    EXPECT_EQ(bda::layoutFromStrides(std::array{1, 4, 2, 1}), bda::TensorLayout::INVALID);

    EXPECT_EQ(bda::layoutFromStrides(std::vector<std::size_t>{1, 2, 4, 8}), bda::TensorLayout::COLUMN_MAJOR);
    EXPECT_EQ(bda::layoutFromStrides(std::vector<std::size_t>{8, 4, 2, 1}), bda::TensorLayout::ROW_MAJOR);
    EXPECT_EQ(bda::layoutFromStrides(std::vector<std::size_t>{1, 4, 2, 1}), bda::TensorLayout::INVALID);
}