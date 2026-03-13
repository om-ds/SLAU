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

TEST(SparseMatrixTest, SolveSimpleIteration)
{
    SparseMatrix<double> A(3, 3);

    A.set(0, 0, 5);
    A.set(1, 1, 5);
    A.set(2, 2, 5);
    A.set(0, 1, 1);
    A.set(0, 2, 1);
    A.set(1, 0, 1);
    A.set(1, 2, 1);
    A.set(2, 0, 1);
    A.set(2, 1, 1);

    std::vector<double> b = {1, 2, 3};
    std::vector<double> ans = A.solveSimpleIteration(b);

    EXPECT_NEAR(ans[0], 0.0357143, 1e-6);
    EXPECT_NEAR(ans[1], 0.285714, 1e-6);
    EXPECT_NEAR(ans[2], 0.535714, 1e-6);
}

TEST(SparseMatrixTest, SolveJacobi)
{
    SparseMatrix<double> A(3, 3);

    A.set(0, 0, 5);
    A.set(1, 1, 5);
    A.set(2, 2, 5);
    A.set(0, 1, 1);
    A.set(0, 2, 1);
    A.set(1, 0, 1);
    A.set(1, 2, 1);
    A.set(2, 0, 1);
    A.set(2, 1, 1);

    std::vector<double> b = {1, 2, 3};
    std::vector<double> ans = A.solveJacobi(b);

    EXPECT_NEAR(ans[0], 0.0357143, 1e-6);
    EXPECT_NEAR(ans[1], 0.285714, 1e-6);
    EXPECT_NEAR(ans[2], 0.535714, 1e-6);
}

TEST(SparseMatrixTest, SolveGaussSeidel)
{
    SparseMatrix<double> A(3, 3);

    A.set(0, 0, 5);
    A.set(1, 1, 5);
    A.set(2, 2, 5);
    A.set(0, 1, 1);
    A.set(0, 2, 1);
    A.set(1, 0, 1);
    A.set(1, 2, 1);
    A.set(2, 0, 1);
    A.set(2, 1, 1);

    std::vector<double> b = {1, 2, 3};
    std::vector<double> ans = A.solveGaussSeidel(b);

    EXPECT_NEAR(ans[0], 0.0357143, 1e-6);
    EXPECT_NEAR(ans[1], 0.285714, 1e-6);
    EXPECT_NEAR(ans[2], 0.535714, 1e-6);
}
