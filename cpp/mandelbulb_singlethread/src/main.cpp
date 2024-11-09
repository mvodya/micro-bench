#include <cmath>
#include <fstream>
#include <iostream>
#include <microbench.hpp>

using namespace MicroBench;

const uint16_t WIDTH = 512, HEIGHT = 512;

int main() {
  Image image(WIDTH, HEIGHT);

  Color *buffer = image.getBuffer();

  for (uint16_t y = 0; y < HEIGHT; y++) {
    for (uint16_t x = 0; x < WIDTH; x++) {
      Color *pixel = buffer + x + (y * WIDTH);
      pixel->r = (uint8_t)((((double)x) / ((double)WIDTH)) * 255.0);
      pixel->g = (uint8_t)((((double)y) / ((double)HEIGHT)) * 255.0);
    }
  }

  image.saveFile("mandelbulb.ppm");

  std::cout << "Image saved as mandelbulb.ppm" << std::endl;
  return 0;
}
