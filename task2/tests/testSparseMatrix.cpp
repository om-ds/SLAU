#include <gtest/gtest.h>
#include "SparseMatrix.hpp"

TEST(SparseMatrixTest, ConstructorAndGetSet)
{
    SparseMatrix<double> m(5, 5);
    EXPECT_EQ(m.get(0, 0), 0.0);
    m.set(2, 3, 7.5);
    EXPECT_EQ(m.get(2, 3), 7.5);
    EXPECT_EQ(m.get(4, 4), 0.0);
}

TEST(SparseMatrixTest, Addition)
{
    SparseMatrix<int> a(3, 3), b(3, 3);
    a.set(0, 0, 1); a.set(1, 1, 2);
    b.set(0, 0, 3); b.set(2, 2, 4);

    SparseMatrix<int> result = a + b;

    EXPECT_EQ(result.get(0, 0), 4);
    EXPECT_EQ(result.get(1, 1), 2);
    EXPECT_EQ(result.get(2, 2), 4);
}

TEST(SparseMatrixTest, MultiplicationByVector)
{
    SparseMatrix<double> m(3, 3);
    m.set(0, 0, 1.0); m.set(1, 1, 2.0); m.set(2, 2, 3.0);

    std::vector<double> v = {1.0, 2.0, 3.0};
    std::vector<double> result = m * v;

    EXPECT_DOUBLE_EQ(result[0], 1.0);
    EXPECT_DOUBLE_EQ(result[1], 4.0);
    EXPECT_DOUBLE_EQ(result[2], 9.0);
}
