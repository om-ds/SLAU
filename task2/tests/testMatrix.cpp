#include <gtest/gtest.h>
#include "Matrix.hpp"

TEST(MatrixTest, ConstructorAndGetSet)
{
    Matrix<double> m(3, 4);
    EXPECT_EQ(m.get(0, 0), 0.0);
    m.set(1, 2, 5.5);
    EXPECT_EQ(m.get(1, 2), 5.5);
}

TEST(MatrixTest, AdditionAndSubtraction)
{
    Matrix<int> a(2, 2), b(2, 2);
    a.set(0, 0, 1); a.set(0, 1, 2); a.set(1, 0, 3); a.set(1, 1, 4);
    b.set(0, 0, 5); b.set(0, 1, 6); b.set(1, 0, 7); b.set(1, 1, 8);

    Matrix<int> sum = a + b;
    Matrix<int> diff = a - b;

    EXPECT_EQ(sum.get(0, 0), 6);
    EXPECT_EQ(diff.get(1, 1), -4);
}

TEST(MatrixTest, MultiplicationByVector)
{
    Matrix<double> m(2, 3);
    m.set(0, 0, 1.0); m.set(0, 1, 2.0); m.set(0, 2, 3.0);
    m.set(1, 0, 4.0); m.set(1, 1, 5.0); m.set(1, 2, 6.0);

    std::vector<double> v = {1.0, 2.0, 3.0};
    std::vector<double> result = m * v;

    EXPECT_DOUBLE_EQ(result[0], 14.0);
    EXPECT_DOUBLE_EQ(result[1], 32.0);
}

