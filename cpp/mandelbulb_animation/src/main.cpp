#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <microbench.hpp>
#include <thread>
#include <vector>
#include <sstream>

#include "mandelbulb_config.h"

using namespace MicroBench;

// Image width ? height in pixels
const uint16_t WIDTH = MB_MANDELBULB_RESOLUTION_WIDTH, HEIGHT = MB_MANDELBULB_RESOLUTION_HEIGHT;

// Maximum number of steps for ray marching
const int MAX_MARCHING_STEPS = MB_MANDELBULB_MAX_MARCHING_STEPS;
// Maximum marching distance before stopping
const double MAX_DISTANCE = MB_MANDELBULB_MAX_DISTANCE;
// Minimum distance threshold to consider on surface
const double SURFACE_DISTANCE = MB_MANDELBULB_SURFACE_DISTANCE;

// Maximum iterations for distance estimator function
const int MAX_ITERATIONS = MB_MANDELBULB_MAX_ITERATIONS;
// Escape radius for distance estimator
const double BAILOUT = MB_MANDELBULB_BAILOUT;
// Power exponent for Mandelbulb fractal shape
const double POWER = MB_MANDELBULB_POWER;

// Animation frames
const size_t FRAMES = 250;

// Distance estimator (DE) function for the Mandelbulb fractal
// Calculate minimum distance from point to fractal
double mandelbulbDE(const Vec3& pos) {
  Vec3 z = pos;
  double dr = 1.0;
  double r = 0.0;

  for (size_t i = 0; i < MAX_ITERATIONS; i++) {
    r = z.length();
    if (r > BAILOUT) break;

    // Convert to spherical coordinates
    double theta = acos(z.z / r);
    double phi = atan2(z.y, z.x);
    double zr = pow(r, POWER);

    // Scale the distance increment
    dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

    // Calculate new coordinates
    double sinTheta = sin(POWER * theta);
    double cosTheta = cos(POWER * theta);
    double sinPhi = sin(POWER * phi);
    double cosPhi = cos(POWER * phi);

    z = Vec3(zr * sinTheta * cosPhi, zr * sinTheta * sinPhi, zr * cosTheta);
    z = z + pos;
  }
  return 0.5 * log(r) * r / dr;
}

// Calculate the normal vector at a surface point
Vec3 getNormal(const Vec3& pos) {
  double eps = 0.001;
  double dx =
      mandelbulbDE(pos + Vec3(eps, 0, 0)) - mandelbulbDE(pos - Vec3(eps, 0, 0));
  double dy =
      mandelbulbDE(pos + Vec3(0, eps, 0)) - mandelbulbDE(pos - Vec3(0, eps, 0));
  double dz =
      mandelbulbDE(pos + Vec3(0, 0, eps)) - mandelbulbDE(pos - Vec3(0, 0, eps));
  Vec3 normal = Vec3(dx, dy, dz);
  return normal.normalize();
}

// Ray marching function to find distance to the surface
double rayMarch(const Vec3& from, const Vec3& dir) {
  double distance = 0.0;
  for (size_t i = 0; i < MAX_MARCHING_STEPS; i++) {
    Vec3 pos = from + dir * distance;
    double d = mandelbulbDE(pos);
    if (d < SURFACE_DISTANCE) {
      // Intersection
      return distance;
    }
    if (distance > MAX_DISTANCE) {
      // No intersection
      break;
    }
    distance += d;
  }
  // No intersection found
  return -1.0;
}

// Function to render a portion of the image
void renderPart(Image* image, uint16_t y_start, uint16_t y_end,
                const Vec3& cameraPos, const Vec3& lightDir) {
  Color* buffer = image->getBuffer();

  for (uint16_t y = y_start; y < y_end; y++) {
    for (uint16_t x = 0; x < WIDTH; x++) {
      // Normalize pixel coordinates
      double px = (x / (double)WIDTH) * 2.0 - 1.0;
      double py = (y / (double)HEIGHT) * 2.0 - 1.0;
      // Adjust for aspect ratio
      px *= (double)WIDTH / (double)HEIGHT;

      // Calculate ray direction
      Vec3 dir = Vec3(px, py, 1.0).normalize();

      double dist = rayMarch(cameraPos, dir);
      if (dist > 0.0) {
        // Calculate intersection point and normal
        Vec3 pos = cameraPos + dir * dist;
        Vec3 normal = getNormal(pos);

        // Compute lighting (Lambertian shading)
        double diffuse = std::max(0.0, normal * lightDir);

        // Set pixel color based on lighting
        uint8_t color = (uint8_t)(diffuse * 255);
        buffer[x + y * WIDTH] = Color(color, color, color);
      } else {
        // Set background color
        buffer[x + y * WIDTH] = Color(0, 0, 0);
      }
    }
  }
}

// Number of threads to use
uint16_t num_threads = 0;

Image* mandelbulb(size_t iteration) {
  Image* image = new Image(WIDTH, HEIGHT);
  double angle = (iteration / (double)FRAMES) * 2.0 * 3.14159217;
  double movement = ((double)iteration / (double)FRAMES) * 1.4;
  // double movement = ((double) iteration / 100.) * 1.4;
  // Vec3 cameraPos(0.0, movement, -1.6 + movement);
  Vec3 cameraPos(cos(angle) * 2.0, movement - 0.5, sin(angle) - 2.4);
  Vec3 lightDir = Vec3(1.0, 1.0, -1.0).normalize();

  // Compute the number of rows per thread
  uint16_t rows_per_thread = HEIGHT / num_threads;
  std::vector<std::thread> threads;

  for (uint16_t i = 0; i < num_threads; ++i) {
    uint16_t y_start = i * rows_per_thread;
    uint16_t y_end =
        (i == num_threads - 1) ? HEIGHT : y_start + rows_per_thread;

    threads.emplace_back(renderPart, image, y_start, y_end, cameraPos,
                         lightDir);
  }

  // Join threads
  for (auto& t : threads) {
    t.join();
  }

  return image;
}

int main() {
  // Get number of threads to use
  num_threads = std::thread::hardware_concurrency();
  // Default to 4 if unable to detect
  if (num_threads == 0) num_threads = 4;

  std::cout << "[CPP] Mandelbulb multi threaded render animation\n";
  std::cout << "Number of available threads: " << num_threads << "\n";
  std::cout << "Size: " << WIDTH << " x " << HEIGHT << "\nPOWER: " << POWER
            << " / MAX ITERATIONS: " << MAX_MARCHING_STEPS << "\n";

  // Animation frames
  for (size_t i = 0; i < FRAMES; i++) {
    std::cout << "Start rendering frame" << i + 1 << "/" << FRAMES << "\n";
    auto [image, duration] = measureExecutionTime(mandelbulb, i);
    std::cout << "[" << i + 1 << "] Execution time: " << duration << " ns ("
              << (double)duration / 1000000000. << " s)\n";

    std::stringstream filename;
    filename << i << ".ppm";
    // Save image to file
    image->saveFile(filename.str().c_str());
    std::cout << "Frame saved to " << filename.str() << "\n";
    delete image;
  }

  std::cout << "All frames renderings are done\n";
  return 0;
}
