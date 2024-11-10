#include <gtest/gtest.h>

#include <thread>

#include "microbench.hpp"

using namespace MicroBench;

// Function for test
long long int sampleComputation() {
  long long int sum = 0;
  for (int i = 0; i < 1000000; ++i) {
    sum += i;
  }
  return sum;
}

TEST(MeasureTimeTest, MeasureExecutionTimeWithResult) {
  // Start test & measure time
  auto [result, duration] = measureExecutionTime(sampleComputation);

  EXPECT_EQ(result, 499999500000);
  EXPECT_GT(duration, 0);
}

TEST(MeasureTimeTest, MeasureExecutionTimeWithDelay) {
  auto delayedFunc = []() {
    // Delay
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return 42;
  };
  auto [result, duration] = measureExecutionTime(delayedFunc);

  EXPECT_EQ(result, 42);
  EXPECT_GE(duration, 100);
}