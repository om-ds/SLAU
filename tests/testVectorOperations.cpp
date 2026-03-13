#include <gtest/gtest.h>
#include "VectorOperations.hpp"

TEST(VectorOperationsTest, VectorOperations)
{
    std::vector<double> v1 = {1.0, 2.0, 3.0};
    std::vector<double> v1_scaled = v1 * 2.0;
    EXPECT_DOUBLE_EQ(v1_scaled[0], 2.0);
    EXPECT_DOUBLE_EQ(v1_scaled[1], 4.0);
    EXPECT_DOUBLE_EQ(v1_scaled[2], 6.0);

    std::vector<double> v2 = {1.0, 2.0, 3.0};
    std::vector<double> v3 = {4.0, 5.0, 6.0};
    double dot = v2 * v3;
    EXPECT_DOUBLE_EQ(dot, 32.0);

    std::vector<double> v4 = {1.0, 2.0, 3.0};
    std::vector<double> v5 = {4.0, 5.0, 6.0};
    std::vector<double> v_sum = v4 + v5;
    EXPECT_DOUBLE_EQ(v_sum[0], 5.0);
    EXPECT_DOUBLE_EQ(v_sum[1], 7.0);
    EXPECT_DOUBLE_EQ(v_sum[2], 9.0);
}
