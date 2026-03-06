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

TEST(MatrixTest, MatrixMultiplication)
{
    Matrix<double> A(2, 3);
    A.set(0, 0, 1.0); A.set(0, 1, 2.0); A.set(0, 2, 3.0);
    A.set(1, 0, 4.0); A.set(1, 1, 5.0); A.set(1, 2, 6.0);

    Matrix<double> B(3, 2);
    B.set(0, 0, 7.0); B.set(0, 1, 8.0);
    B.set(1, 0, 9.0); B.set(1, 1, 10.0);
    B.set(2, 0, 11.0); B.set(2, 1, 12.0);

    Matrix<double> C = A * B;

    EXPECT_DOUBLE_EQ(C.get(0, 0), 58.0);
    EXPECT_DOUBLE_EQ(C.get(0, 1), 64.0);
    EXPECT_DOUBLE_EQ(C.get(1, 0), 139.0);
    EXPECT_DOUBLE_EQ(C.get(1, 1), 154.0);
}

TEST(MatrixTest, QRDecomposition)
{
    Matrix<double> A(3, 2);
    A.set(0, 0, 1.0); A.set(0, 1, 1.0);
    A.set(1, 0, 1.0); A.set(1, 1, 0.0);
    A.set(2, 0, 1.0); A.set(2, 1, 1.0);
    auto [Q, R] = A.qr();
    EXPECT_NEAR(R.get(1, 0), 0.0, 1e-10);
    EXPECT_NEAR(R.get(2, 0), 0.0, 1e-10);
    EXPECT_NEAR(R.get(2, 1), 0.0, 1e-10);
    Matrix<double> QR = Q * R;
    EXPECT_NEAR(QR.get(0, 0), A.get(0, 0), 1e-10);
    EXPECT_NEAR(QR.get(0, 1), A.get(0, 1), 1e-10);
    EXPECT_NEAR(QR.get(1, 0), A.get(1, 0), 1e-10);
    EXPECT_NEAR(QR.get(1, 1), A.get(1, 1), 1e-10);
    EXPECT_NEAR(QR.get(2, 0), A.get(2, 0), 1e-10);
    EXPECT_NEAR(QR.get(2, 1), A.get(2, 1), 1e-10);
}

TEST(MatrixTest, SolveEquation)
{
    Matrix<double> A(2, 2);
    A.set(0, 0, 2.0); A.set(0, 1, 1.0);
    A.set(1, 0, 1.0); A.set(1, 1, 3.0);

    std::vector<double> b = {4.0, 7.0};
    std::vector<double> x = A.solve(b);

    EXPECT_NEAR(x[0], 1.0, 1e-10);
    EXPECT_NEAR(x[1], 2.0, 1e-10);

    std::vector<double> b_check = A * x;
    EXPECT_NEAR(b_check[0], b[0], 1e-10);
    EXPECT_NEAR(b_check[1], b[1], 1e-10);
}
