#include <gtest/gtest.h>

#include <cmath>

#include "microbench.hpp"

using namespace MicroBench;

// Tolerance for floating-point comparisons
const float TOLERANCE = 1e-5f;

TEST(Vec3Test, Addition) {
  Vec3 v1(1.0f, 2.0f, 3.0f);
  Vec3 v2(4.0f, 5.0f, 6.0f);
  Vec3 result = v1 + v2;
  EXPECT_NEAR(result.x, 5.0f, TOLERANCE);
  EXPECT_NEAR(result.y, 7.0f, TOLERANCE);
  EXPECT_NEAR(result.z, 9.0f, TOLERANCE);
}

TEST(Vec3Test, Subtraction) {
  Vec3 v1(4.0f, 5.0f, 6.0f);
  Vec3 v2(1.0f, 2.0f, 3.0f);
  Vec3 result = v1 - v2;
  EXPECT_NEAR(result.x, 3.0f, TOLERANCE);
  EXPECT_NEAR(result.y, 3.0f, TOLERANCE);
  EXPECT_NEAR(result.z, 3.0f, TOLERANCE);
}

TEST(Vec3Test, ScalarMultiplication) {
  Vec3 v(1.0f, 2.0f, 3.0f);
  Vec3 result = v * 2.0f;
  EXPECT_NEAR(result.x, 2.0f, TOLERANCE);
  EXPECT_NEAR(result.y, 4.0f, TOLERANCE);
  EXPECT_NEAR(result.z, 6.0f, TOLERANCE);
}

TEST(Vec3Test, Length) {
  Vec3 v(3.0f, 4.0f, 0.0f);
  EXPECT_NEAR(v.length(), 5.0f, TOLERANCE);
}

TEST(Vec3Test, Normalization) {
  Vec3 v(3.0f, 0.0f, 4.0f);
  Vec3 result = v.normalized();
  EXPECT_NEAR(result.x, 0.6f, TOLERANCE);
  EXPECT_NEAR(result.y, 0.0f, TOLERANCE);
  EXPECT_NEAR(result.z, 0.8f, TOLERANCE);
  EXPECT_NEAR(result.length(), 1.0f, TOLERANCE);
}
