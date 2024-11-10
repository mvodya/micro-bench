#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>

#include "microbench.hpp"

using ::testing::HasSubstr;
using ::testing::StartsWith;

// Tolerance for floating-point comparisons
const float TOLERANCE = 1e-5f;

TEST(ImageTest, ImageSizes) {
  MicroBench::Image *image0 = new MicroBench::Image(1920, 1080);
  ASSERT_EQ(image0->getWidth(), 1920);
  ASSERT_EQ(image0->getHeight(), 1080);

  MicroBench::Image *image1 = new MicroBench::Image(512, 512);
  ASSERT_EQ(image1->getWidth(), 512);
  ASSERT_EQ(image1->getHeight(), 512);

  MicroBench::Image *image2 = new MicroBench::Image(4, 3);
  ASSERT_EQ(image2->getWidth(), 4);
  ASSERT_EQ(image2->getHeight(), 3);

  MicroBench::Image *image3 = new MicroBench::Image(3, 4);
  ASSERT_NE(image3->getWidth(), 4);
  ASSERT_NE(image3->getHeight(), 3);
}

TEST(ImageDestructionTest, DestructorDoesNotLeakMemory) {
  for (int i = 0; i < 1000; ++i) {
    MicroBench::Image image(512, 512);
    MicroBench::Color *buffer = image.getBuffer();

    ASSERT_NE(buffer, nullptr);

    image.setPixel(0, 0, MicroBench::Color(255, 0, 0));
    ASSERT_EQ(image.getPixel(0, 0).r, 255);
    ASSERT_EQ(image.getPixel(0, 0).g, 0);
    ASSERT_EQ(image.getPixel(0, 0).b, 0);
  }
}

TEST(ImageTest, ImageInvalidSizes) {
  ASSERT_THROW(
      { MicroBench::Image *image0 = new MicroBench::Image(0, 0); },
      std::invalid_argument);

  ASSERT_THROW(
      { MicroBench::Image *image1 = new MicroBench::Image(0, 100); },
      std::invalid_argument);

  ASSERT_THROW(
      { MicroBench::Image *image2 = new MicroBench::Image(100, 0); },
      std::invalid_argument);

  ASSERT_THROW(
      { MicroBench::Image *image3 = new MicroBench::Image(-100, 0); },
      std::invalid_argument);

  ASSERT_THROW(
      { MicroBench::Image *image3 = new MicroBench::Image(0, -100); },
      std::invalid_argument);
}

TEST(ImageTest, ImageInitZero) {
  const size_t SIZE = 32;
  MicroBench::Image *image = new MicroBench::Image(SIZE, SIZE);

  MicroBench::Color *buffer = image->getBuffer();

  // Check zeros in buffer
  for (size_t i = 0; i < SIZE * SIZE; i++) {
    MicroBench::Color pixel = *buffer;
    ASSERT_EQ(pixel.r, 0);
    ASSERT_EQ(pixel.g, 0);
    ASSERT_EQ(pixel.b, 0);
  }
}

TEST(ImageTest, ImageInitFill) {
  const size_t SIZE_X = 512;
  const size_t SIZE_Y = 1024;
  MicroBench::Image *image = new MicroBench::Image(SIZE_X, SIZE_Y);

  MicroBench::Color *buffer = image->getBuffer();

  // Set
  for (size_t i = 0; i < SIZE_X * SIZE_Y; i++) {
    MicroBench::Color *pixel = buffer;
    pixel->r = 1;
    pixel->g = 2;
    pixel->b = 3;
  }

  // Check
  for (size_t i = 0; i < SIZE_X * SIZE_Y; i++) {
    MicroBench::Color pixel = *buffer;
    ASSERT_EQ(pixel.r, 1);
    ASSERT_EQ(pixel.g, 2);
    ASSERT_EQ(pixel.b, 3);
  }
}

TEST(ImageTest, ImageInitFillGetSet) {
  const size_t SIZE_X = 512;
  const size_t SIZE_Y = 1024;
  MicroBench::Image *image = new MicroBench::Image(SIZE_X, SIZE_Y);

  // Set
  for (uint16_t i = 0; i < SIZE_X; i++) {
    for (uint16_t j = 0; j < SIZE_Y; j++) {
      MicroBench::Color pixel(i % 255, j % 255, i * j % 100);
      image->setPixel(i, j, pixel);
    }
  }

  // Check
  for (uint16_t i = 0; i < SIZE_X; i++) {
    for (uint16_t j = 0; j < SIZE_Y; j++) {
      MicroBench::Color pixel = image->getPixel(i, j);
      ASSERT_EQ(pixel.r, i % 255);
      ASSERT_EQ(pixel.g, j % 255);
      ASSERT_EQ(pixel.b, i * j % 100);
    }
  }
}

TEST(ImageTest, ImageGetSetPixelPtr) {
  MicroBench::Image *image = new MicroBench::Image(32, 32);

  MicroBench::Color *pixel = image->getPixelPtr(16, 5);
  ASSERT_EQ(pixel->r, 0);
  ASSERT_EQ(pixel->g, 0);
  ASSERT_EQ(pixel->b, 0);

  pixel->r = 250;
  pixel->g = 120;
  pixel->b = 94;

  MicroBench::Color *pixelCheck = image->getPixelPtr(16, 5);
  ASSERT_EQ(pixel->r, 250);
  ASSERT_EQ(pixel->g, 120);
  ASSERT_EQ(pixel->b, 94);
}

TEST(ImageTest, ImageGetSetPixel) {
  MicroBench::Image *image = new MicroBench::Image(32, 32);

  MicroBench::Color pixel = image->getPixel(30, 15);
  ASSERT_EQ(pixel.r, 0);
  ASSERT_EQ(pixel.g, 0);
  ASSERT_EQ(pixel.b, 0);

  image->setPixel(30, 15, MicroBench::Color(210, 170, 60));

  MicroBench::Color pixelCheck = image->getPixel(30, 15);
  ASSERT_EQ(pixelCheck.r, 210);
  ASSERT_EQ(pixelCheck.g, 170);
  ASSERT_EQ(pixelCheck.b, 60);
}

TEST(ImageTest, ImageGetSetCheckBounds) {
  MicroBench::Image *image0 = new MicroBench::Image(1000, 2000);

  ASSERT_NO_THROW({
    image0->getPixel(999, 1999);
    image0->setPixel(999, 1999, MicroBench::Color(100, 200, 150));
  });

  ASSERT_THROW({ image0->getPixel(1000, 0); }, std::out_of_range);

  ASSERT_THROW({ image0->getPixel(0, 2000); }, std::out_of_range);

  ASSERT_THROW({ image0->getPixel(1500, 0); }, std::out_of_range);

  ASSERT_THROW({ image0->getPixel(0, 2500); }, std::out_of_range);

  ASSERT_THROW({ image0->getPixel(-1, 0); }, std::out_of_range);

  ASSERT_THROW({ image0->getPixel(0, -1); }, std::out_of_range);

  ASSERT_THROW({ image0->getPixel(1500, 2500); }, std::out_of_range);
}

TEST(ImageTest, ImageGeneratePPM) {
  MicroBench::Image *image = new MicroBench::Image(32, 16);

  image->setPixel(30, 15, MicroBench::Color(0xFF, 0x10, 0x05));

  // Generate PPM
  std::stringstream ss;
  image->generatePPM(ss);

  std::string data = ss.str();

  // Check header
  ASSERT_THAT(data, StartsWith("P6\n# Microbench lib\n32 16\n255\n"));
  // Check pixel
  ASSERT_THAT(data, HasSubstr("\xFF\x10\x05"));
}

TEST(ImageTest, ColorToVec3Conversion) {
  MicroBench::Color color(128, 64, 255);
  MicroBench::Vec3 vec = color;
  EXPECT_NEAR(vec.x, 128.0 / 255.0, TOLERANCE);
  EXPECT_NEAR(vec.y, 64.0 / 255.0, TOLERANCE);
  EXPECT_NEAR(vec.z, 255.0 / 255.0, TOLERANCE);
}

TEST(ImageTest, DefaultColorToVec3Conversion) {
  MicroBench::Color color;
  MicroBench::Vec3 vec = color;
  EXPECT_NEAR(vec.x, 0.0, TOLERANCE);
  EXPECT_NEAR(vec.y, 0.0, TOLERANCE);
  EXPECT_NEAR(vec.z, 0.0, TOLERANCE);
}
