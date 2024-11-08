#include <stdexcept>

#include "microbench.hpp"

using namespace MicroBench;

Image::Image(uint16_t width, uint16_t height) {
  this->_width = width;
  this->_height = height;

  uint32_t _pixels = width + height;

  this->_pixelBuffer = new Color[this->_width * this->_height];
  memset(this->_pixelBuffer, 0, sizeof(Color));
}

uint16_t Image::getWidth() { return this->_width; }

uint16_t Image::getHeight() { return this->_height; }

Color* Image::getBuffer() { return this->_pixelBuffer; }

Color* Image::getPixelPtr(uint16_t x, uint16_t y) {
  // Check bounds
  if (x >= this->_width)
    throw std::out_of_range("Image::getPixel - x is too big");
  if (y >= this->_height)
    throw std::out_of_range("Image::getPixel - y is too big");

  // Get by column & row
  return &(this->_pixelBuffer[x + (y * this->_height)]);
}

Color Image::getPixel(uint16_t x, uint16_t y) {
  // Check bounds
  if (x >= this->_width)
    throw std::out_of_range("Image::getPixel - x is too big");
  if (y >= this->_height)
    throw std::out_of_range("Image::getPixel - y is too big");

  // Get by column & row
  return this->_pixelBuffer[x + (y * this->_width)];
}

void Image::setPixel(uint16_t x, uint16_t y, Color color) {
  // Check bounds
  if (x >= this->_width)
    throw std::out_of_range("Image::getPixel - x is too big");
  if (y >= this->_height)
    throw std::out_of_range("Image::getPixel - y is too big");

  // Set by column & row
  this->_pixelBuffer[x + (y * this->_width)] = color;
}

void Image::savePPM(std::ostream& out) {
  // Add header info
  out << "P6\n" << this->_width << " " << this->_height << "\n255\n";

  const uint32_t pixelCount = this->_width * this->_height;
  for (uint32_t i = 0; i < pixelCount; i++) {
    Color* pixel = &this->_pixelBuffer[i];
    out << pixel->r << pixel->g << pixel->b;
  }
}