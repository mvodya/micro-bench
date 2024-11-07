#include <fstream>
#include <iostream>
#include <microbench.hpp>

int main() {
  MicroBench::Image image(32, 32);

  // Demo paint
  for (uint16_t i = 0; i < 32; i++) {
    for (uint16_t j = 0; j < 10; j++) {
      image.setPixel(i, j, MicroBench::Color(i > 16 ? 0xFF : 0x10, j > 16 ? 0x10 : 0xFF, 0x00));
    }
  }

  // Save image file
  std::ofstream file("image.ppm");
  image.savePPM(file);
  file.close();

  return 0;
}