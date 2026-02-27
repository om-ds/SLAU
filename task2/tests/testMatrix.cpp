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

TEST(MatrixTest, VectorOperations)
{
    std::vector<double> v1 = {1.0, 2.0, 3.0};
    std::vector<double> v1_scaled = v1 * 2.0;
    EXPECT_DOUBLE_EQ(v1_scaled[0], 2.0);
    EXPECT_DOUBLE_EQ(v1_scaled[1], 4.0);
    EXPECT_DOUBLE_EQ(v1_scaled[2], 6.0);

    std::vector<double> v2 = {1.0, 2.0, 3.0};
    std::vector<double> v3 = {4.0, 5.0, 6.0};
    double dot = v2 * v3;
    EXPECT_DOUBLE_EQ(dot, 32.0); // 1*4 + 2*5 + 3*6 = 32

    std::vector<double> v4 = {1.0, 2.0, 3.0};
    std::vector<double> v5 = {4.0, 5.0, 6.0};
    std::vector<double> v_sum = v4 + v5;
    EXPECT_DOUBLE_EQ(v_sum[0], 5.0);
    EXPECT_DOUBLE_EQ(v_sum[1], 7.0);
    EXPECT_DOUBLE_EQ(v_sum[2], 9.0);
}
