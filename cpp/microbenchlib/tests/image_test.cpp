#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>

#include "microbench.hpp"

using ::testing::StartsWith;
using ::testing::HasSubstr;

TEST(MicrobenchLibTest, ImageSizes) {
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

TEST(MicrobenchLibTest, ImageInitZero) {
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

TEST(MicrobenchLibTest, ImageInitFill) {
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

TEST(MicrobenchLibTest, ImageInitFillGetSet) {
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

TEST(MicrobenchLibTest, ImageGetSetPixelPtr) {
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

TEST(MicrobenchLibTest, ImageGetSetPixel) {
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

TEST(MicrobenchLibTest, ImageGetSetCheckBounds) {
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

TEST(MicrobenchLibTest, ImageSavePPM) {
  MicroBench::Image *image = new MicroBench::Image(32, 16);

  image->setPixel(30, 15, MicroBench::Color(0xFF, 0x10, 0x05));

  // Generate PPM
  std::stringstream ss;
  image->savePPM(ss);

  std::string data = ss.str();

  // Check header
  ASSERT_THAT(data, StartsWith("P3\n32 16\n255\n"));
  // Check pixel
  ASSERT_THAT(data, HasSubstr("\xFF \x10 \x05\n"));
}