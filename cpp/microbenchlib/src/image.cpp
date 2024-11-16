#include <fstream>
#include <stdexcept>
#include <string.h>

#include "microbench.hpp"

using namespace MicroBench;

Color::operator Vec3() const { return Vec3(r / 255.0, g / 255.0, b / 255.0); }

Image::Image(uint16_t width, uint16_t height) {
  if (width <= 0) throw std::invalid_argument("width is invalid size");
  if (height <= 0) throw std::invalid_argument("height is invalid size");

  _width = width;
  _height = height;

  uint32_t _pixels = width + height;

  _pixelBuffer = new Color[_width * _height];
  memset(_pixelBuffer, 0, sizeof(Color));
}

Image::~Image() { delete[] _pixelBuffer; }

uint16_t Image::getWidth() { return _width; }

uint16_t Image::getHeight() { return _height; }

Color* Image::getBuffer() { return _pixelBuffer; }

Color* Image::getPixelPtr(uint16_t x, uint16_t y) {
  // Check bounds
  if (x >= _width) throw std::out_of_range("Image::getPixel - x is too big");
  if (y >= _height) throw std::out_of_range("Image::getPixel - y is too big");

  // Get by column & row
  return &(_pixelBuffer[x + (y * _height)]);
}

Color Image::getPixel(uint16_t x, uint16_t y) {
  // Check bounds
  if (x >= _width) throw std::out_of_range("Image::getPixel - x is too big");
  if (y >= _height) throw std::out_of_range("Image::getPixel - y is too big");

  // Get by column & row
  return _pixelBuffer[x + (y * _width)];
}

void Image::setPixel(uint16_t x, uint16_t y, Color color) {
  // Check bounds
  if (x >= _width) throw std::out_of_range("Image::getPixel - x is too big");
  if (y >= _height) throw std::out_of_range("Image::getPixel - y is too big");

  // Set by column & row
  _pixelBuffer[x + (y * _width)] = color;
}

void Image::generatePPM(std::ostream& out) {
  // Add header info
  out << "P6\n# Microbench lib\n" << _width << " " << _height << "\n255\n";

  const uint32_t pixelCount = _width * _height;
  for (uint32_t i = 0; i < pixelCount; i++) {
    Color* pixel = &_pixelBuffer[i];
    out << pixel->r << pixel->g << pixel->b;
  }
}

bool Image::saveFile(const char* filename) {
  std::ofstream file(filename, std::ios::binary | std::ios::out);
  // Check is file is open
  if (!file.is_open()) return false;

  // Generate PPM
  generatePPM(file);
  file.close();

  return true;
}