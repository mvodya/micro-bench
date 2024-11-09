#include <cstdint>
#include <iostream>

namespace MicroBench {

// [RED] [GREEN] [BLUE]
// r     g       b
struct Color {
  uint8_t r, g, b;

  Color() : r(0), g(0), b(0) {}
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

// Image with RGB pixel buffer
//
//  %--- WIDTH / x --→
//  |                *
//  |                *
//  HEIGHT           *
//  y                *
//  |                *
//  ↓ *  *  *  *  *  *
//
class Image {
  // Image sizes
  // _width  - x
  // _height - y
  uint16_t _width, _height;

  // Pixels
  Color* _pixelBuffer;

 public:
  Image(uint16_t width, uint16_t height);

  // Get image size
  uint16_t getWidth();
  uint16_t getHeight();

  // Get image pixel buffer
  Color* getBuffer();

  // Get image pixel pointer
  Color* getPixelPtr(uint16_t x, uint16_t y);

  // Get image pixel
  Color getPixel(uint16_t x, uint16_t y);
  // Set image pixel
  void setPixel(uint16_t x, uint16_t y, Color color);

  // Generate image in PPM (Portable PixMap) format
  void generatePPM(std::ostream& out);

  // Save image file in PPM (Portable PixMap) format
  bool Image::saveFile(const char* filename);
};

}  // namespace MicroBench