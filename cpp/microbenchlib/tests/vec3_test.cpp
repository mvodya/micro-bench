#include <gtest/gtest.h>

#include <cmath>

#include "microbench.hpp"

using namespace MicroBench;

// Tolerance for floating-point comparisons
const float TOLERANCE = 1e-5f;

TEST(Vec3Test, CopyColor) {
  Vec3 v(1.0, 2.0, 3.0);
  Vec3 result = v;
  EXPECT_NEAR(result.x, 1.0, TOLERANCE);
  EXPECT_NEAR(result.y, 2.0, TOLERANCE);
  EXPECT_NEAR(result.z, 3.0, TOLERANCE);
}

TEST(Vec3Test, Addition) {
  Vec3 v1(1.0, 2.0, 3.0);
  Vec3 v2(4.0, 5.0, 6.0);
  Vec3 result = v1 + v2;
  EXPECT_NEAR(result.x, 5.0, TOLERANCE);
  EXPECT_NEAR(result.y, 7.0, TOLERANCE);
  EXPECT_NEAR(result.z, 9.0, TOLERANCE);
}

TEST(Vec3Test, Subtraction) {
  Vec3 v1(4.0, 5.0, 6.0);
  Vec3 v2(1.0, 2.0, 3.0);
  Vec3 result = v1 - v2;
  EXPECT_NEAR(result.x, 3.0, TOLERANCE);
  EXPECT_NEAR(result.y, 3.0, TOLERANCE);
  EXPECT_NEAR(result.z, 3.0, TOLERANCE);
}

TEST(Vec3Test, Multiplication) {
  Vec3 v(1.0, 2.0, 3.0);
  Vec3 result = v * 2.0;
  EXPECT_NEAR(result.x, 2.0, TOLERANCE);
  EXPECT_NEAR(result.y, 4.0, TOLERANCE);
  EXPECT_NEAR(result.z, 6.0, TOLERANCE);
}

TEST(Vec3Test, ScalarMultiplication) {
  Vec3 v1(1.0, 2.0, 3.0);
  Vec3 v2(4.0, -5.0, 6.0);
  double result = v1 * v2;
  EXPECT_NEAR(result, 12.0, TOLERANCE);
}

TEST(Vec3Test, Length) {
  Vec3 v(3.0, 4.0, 0.0);
  EXPECT_NEAR(v.length(), 5.0, TOLERANCE);
}

TEST(Vec3Test, Normalization) {
  Vec3 v(3.0, 0.0, 4.0);
  Vec3 result = v.normalize();
  EXPECT_NEAR(result.x, 0.6, TOLERANCE);
  EXPECT_NEAR(result.y, 0.0, TOLERANCE);
  EXPECT_NEAR(result.z, 0.8, TOLERANCE);
  EXPECT_NEAR(result.length(), 1.0, TOLERANCE);
}

TEST(Vec3Test, Vec3ToColorConversion) {
  Vec3 vec(0.5, 0.25, 1.0);
  Color color = vec;
  EXPECT_EQ(color.r, static_cast<uint8_t>(0.5 * 255));
  EXPECT_EQ(color.g, static_cast<uint8_t>(0.25 * 255));
  EXPECT_EQ(color.b, static_cast<uint8_t>(1.0 * 255));
}