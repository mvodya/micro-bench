#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <microbench.hpp>

using namespace MicroBench;

// Image width ? height in pixels
const uint16_t WIDTH = 2048, HEIGHT = 2048;

// Maximum number of steps for ray marching
const int MAX_MARCHING_STEPS = 100;
// Maximum marching distance before stopping
const double MAX_DISTANCE = 100.0;
// Minimum distance threshold to consider on surface
const double SURFACE_DISTANCE = 0.001;

// Maximum iterations for distance estimator function
const int MAX_ITERATIONS = 8;
// Escape radius for distance estimator
const double BAILOUT = 2.0;
// Power exponent for Mandelbulb fractal shape
const double POWER = 8.0;

// Distance estimator (DE) function for the Mandelbulb fractal
// Calculate minimum distance from point to fractal
double mandelbulbDE(const Vec3& pos) {
  // Position
  Vec3 z = pos;
  // Distance increment
  double dr = 1.0;
  // Distance
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

Image* mandelbulb() {
  Image* image = new Image(WIDTH, HEIGHT);
  Color* buffer = image->getBuffer();

  Vec3 cameraPos(0.0, 0.0, -1.6);
  Vec3 lightDir = Vec3(1.0, 1.0, -1.0).normalize();

  for (uint16_t y = 0; y < HEIGHT; y++) {
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

  return image;
}

int main() {
  const size_t STEPS = 10;

  std::cout << "Mandelbulb single thread benchmark\n";
  std::cout << "Size: " << WIDTH << " x " << HEIGHT << "\nPOWER: " << POWER
            << " / MAX ITERATIONS: " << MAX_MARCHING_STEPS << "\n";

  // Steps
  for (size_t i = 0; i < STEPS; i++) {
    std::cout << "Start rendering " << i + 1 << "/" << STEPS << "\n";
    auto [image, duration] = measureExecutionTime(mandelbulb);
    std::cout << "[" << i + 1 << "] Execution time: " << duration << " ns ("
              << (double)duration / 1000000000. << " s)\n";

    if (i == 0) {
      std::cout << "Save first run to file mandelbulb.ppm\n";
      // Save image to file
      image->saveFile("mandelbulb.ppm");
    }
    delete image;
  }

  std::cout << "All renderings are done\n";
  return 0;
}
