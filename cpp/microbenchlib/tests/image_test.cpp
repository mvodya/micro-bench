#include <gtest/gtest.h>

#include "microbench.hpp"

TEST(MicrobenchLibTest, ImageSizes) {
  MicroBench::Image *image0 = new MicroBench::Image(1920, 1080);
  EXPECT_EQ(image0->getWidth(), 1920);
  EXPECT_EQ(image0->getHeight(), 1080);

  MicroBench::Image *image1 = new MicroBench::Image(512, 512);
  EXPECT_EQ(image1->getWidth(), 512);
  EXPECT_EQ(image1->getHeight(), 512);

  MicroBench::Image *image2 = new MicroBench::Image(4, 3);
  EXPECT_EQ(image2->getWidth(), 4);
  EXPECT_EQ(image2->getHeight(), 3);

  MicroBench::Image *image3 = new MicroBench::Image(3, 4);
  EXPECT_NE(image3->getWidth(), 4);
  EXPECT_NE(image3->getHeight(), 3);
}

TEST(MicrobenchLibTest, ImageInitZero) {
  const size_t SIZE = 32;
  MicroBench::Image *image = new MicroBench::Image(SIZE, SIZE);

  MicroBench::Color *buffer = image->getBuffer();

  // Check zeros in buffer
  for (int i = 0; i < SIZE * SIZE; i++) {
    MicroBench::Color pixel = *buffer;
    EXPECT_EQ(pixel.r, 0);
    EXPECT_EQ(pixel.g, 0);
    EXPECT_EQ(pixel.b, 0);
  }
}

TEST(MicrobenchLibTest, ImageInitFill) {
  const size_t SIZE = 32;
  MicroBench::Image *image = new MicroBench::Image(SIZE, SIZE);

  MicroBench::Color *buffer = image->getBuffer();

  // Set
  for (int i = 0; i < SIZE * SIZE; i++) {
    MicroBench::Color *pixel = buffer;
    pixel->r = 1;
    pixel->g = 2;
    pixel->b = 3;
  }

  // Check
  for (int i = 0; i < SIZE * SIZE; i++) {
    MicroBench::Color pixel = *buffer;
    EXPECT_EQ(pixel.r, 1);
    EXPECT_EQ(pixel.g, 2);
    EXPECT_EQ(pixel.b, 3);
  }
}

TEST(MicrobenchLibTest, ImageGetSetPixelPtr) {
  MicroBench::Image *image = new MicroBench::Image(32, 32);

  MicroBench::Color *pixel = image->getPixelPtr(16, 5);
  EXPECT_EQ(pixel->r, 0);
  EXPECT_EQ(pixel->g, 0);
  EXPECT_EQ(pixel->b, 0);

  pixel->r = 250;
  pixel->g = 120;
  pixel->b = 94;

  MicroBench::Color *pixelCheck = image->getPixelPtr(16, 5);
  EXPECT_EQ(pixel->r, 250);
  EXPECT_EQ(pixel->g, 120);
  EXPECT_EQ(pixel->b, 94);
}

TEST(MicrobenchLibTest, ImageGetSetPixel) {
  MicroBench::Image *image = new MicroBench::Image(32, 32);

  MicroBench::Color pixel = image->getPixel(30, 15);
  EXPECT_EQ(pixel.r, 0);
  EXPECT_EQ(pixel.g, 0);
  EXPECT_EQ(pixel.b, 0);

  image->setPixel(30, 15, MicroBench::Color(210, 170, 60));

  MicroBench::Color pixelCheck = image->getPixel(30, 15);
  EXPECT_EQ(pixelCheck.r, 210);
  EXPECT_EQ(pixelCheck.g, 170);
  EXPECT_EQ(pixelCheck.b, 60);
}