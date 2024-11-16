#include <cuda_runtime.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <microbench.hpp>

#include "mandelbulb_config.h"

using namespace MicroBench;

// Image width ? height in pixels
const uint16_t WIDTH = MB_MANDELBULB_RESOLUTION_WIDTH, HEIGHT = MB_MANDELBULB_RESOLUTION_HEIGHT;

// Maximum number of steps for ray marching
__device__ const int MAX_MARCHING_STEPS = MB_MANDELBULB_MAX_MARCHING_STEPS;
// Maximum marching distance before stopping
__device__ const double MAX_DISTANCE = MB_MANDELBULB_MAX_DISTANCE;
// Minimum distance threshold to consider on surface
__device__ const double SURFACE_DISTANCE = MB_MANDELBULB_SURFACE_DISTANCE;

// Maximum iterations for distance estimator function
__device__ const int MAX_ITERATIONS = MB_MANDELBULB_MAX_ITERATIONS;
// Escape radius for distance estimator
__device__ const double BAILOUT = MB_MANDELBULB_BAILOUT;
// Power exponent for Mandelbulb fractal shape
__device__ const double POWER = MB_MANDELBULB_POWER;

__device__ const double EPS = 0.001;

// Custom CVec3 class for CUDA
struct CVec3 {
  // Vector values
  double x, y, z;

  __device__ CVec3(double x = 0, double y = 0, double z = 0)
      : x(x), y(y), z(z) {}

  // Summation
  __device__ CVec3 operator+(const CVec3& v) const {
    return CVec3(x + v.x, y + v.y, z + v.z);
  }
  // Subtraction
  __device__ CVec3 operator-(const CVec3& v) const {
    return CVec3(x - v.x, y - v.y, z - v.z);
  }
  // Multiplication by value
  __device__ CVec3 operator*(double s) const {
    return CVec3(x * s, y * s, z * s);
  }
  // Scalar multiplication
  __device__ double operator*(const CVec3& v) const {
    return x * v.x + y * v.y + z * v.z;
  }
  // Calculate vector length
  __device__ double length() const { return sqrt(x * x + y * y + z * z); }
  // Get vector normalize
  __device__ CVec3 normalize() const {
    double len = sqrt(x * x + y * y + z * z);
    return CVec3(x / len, y / len, z / len);
  }
};

// Distance estimator (DE) function for the Mandelbulb fractal
// Calculate minimum distance from point to fractal
__device__ double mandelbulbDE(const CVec3& pos) {
  // Position
  CVec3 z = pos;
  // Distance increment
  double dr = 1.0;
  // Distance
  double r = 0.0;

  for (int i = 0; i < MAX_ITERATIONS; ++i) {
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

    z = CVec3(zr * sinTheta * cosPhi, zr * sinTheta * sinPhi, zr * cosTheta);
    z = z + pos;
  }
  return 0.5 * log(r) * r / dr;
}

// Calculate the normal vector at a surface point
__device__ CVec3 getNormal(const CVec3& pos) {
  double dx = mandelbulbDE(pos + CVec3(EPS, 0, 0)) -
              mandelbulbDE(pos - CVec3(EPS, 0, 0));
  double dy = mandelbulbDE(pos + CVec3(0, EPS, 0)) -
              mandelbulbDE(pos - CVec3(0, EPS, 0));
  double dz = mandelbulbDE(pos + CVec3(0, 0, EPS)) -
              mandelbulbDE(pos - CVec3(0, 0, EPS));
  CVec3 normal = CVec3(dx, dy, dz);
  return normal.normalize();
}

// Ray marching function to find distance to the surface
__device__ double rayMarch(const CVec3& from, const CVec3& dir) {
  double distance = 0.0;
  for (int i = 0; i < MAX_MARCHING_STEPS; ++i) {
    CVec3 pos = from + dir * distance;
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

// CUDA kernel for Mandelbulb rendering
__global__ void mandelbulbKernel(uint8_t* buffer) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;
  if (x >= WIDTH || y >= HEIGHT) return;

  int idx = (y * WIDTH + x) * 3;
  CVec3 cameraPos(0.0, 0.0, -1.6);
  CVec3 lightDir = CVec3(1.0, 1.0, -1.0).normalize();

  // Normalize pixel coordinates
  double px = (x / (double)WIDTH) * 2.0 - 1.0;
  double py = (y / (double)HEIGHT) * 2.0 - 1.0;
  // Adjust for aspect ratio
  px *= (double)WIDTH / (double)HEIGHT;

  // Calculate ray direction
  CVec3 dir = CVec3(px, py, 1.0).normalize();

  double dist = rayMarch(cameraPos, dir);
  if (dist > 0.0) {
    // Calculate intersection point and normal
    CVec3 pos = cameraPos + dir * dist;
    CVec3 normal = getNormal(pos);

    // Compute lighting (Lambertian shading)
    double diffuse = max(0.0, normal * lightDir);

    // Set pixel color based on lighting
    uint8_t color = (uint8_t)(diffuse * 255);
    buffer[idx] = color;
    buffer[idx + 1] = color;
    buffer[idx + 2] = color;
  } else {
    // Set background color
    buffer[idx] = buffer[idx + 1] = buffer[idx + 2] = 0;
  }
}

int main() {
  const size_t STEPS = MB_RENDER_NUM;
  std::cout << "[CPP CUDA] Mandelbulb GPU benchmark\n";
  std::cout << "Size: " << WIDTH << " x " << HEIGHT << "\nPOWER: " << POWER
            << " / MAX ITERATIONS: " << MAX_MARCHING_STEPS << "\nCUDA Block: 16 x 16 = 256\n";

  uint8_t* d_buffer;
  // Allocate buffer on GPU device
  cudaMalloc(&d_buffer, WIDTH * HEIGHT * 3);

  // 16 per 16 threads = 256 threads in one block
  dim3 blockSize(16, 16);
  dim3 gridSize((WIDTH + blockSize.x - 1) / blockSize.x,
                (HEIGHT + blockSize.y - 1) / blockSize.y);

  // Steps
  for (size_t i = 0; i < STEPS; i++) {
    std::cout << "Start rendering " << i + 1 << "/" << STEPS << "\n";
    auto duration = measureExecutionTime([&]() {
      mandelbulbKernel<<<gridSize, blockSize>>>(d_buffer);
      cudaDeviceSynchronize();
    });
    std::cout << "[" << i + 1 << "] Execution time: " << duration << " ns ("
              << (double)duration / 1000000000. << " s)\n";

    if (i == 0) {
      std::cout << "Save first run to file mandelbulb.ppm\n";
      Image* image = new Image(WIDTH, HEIGHT);
      cudaMemcpy(image->getBuffer(), d_buffer, WIDTH * HEIGHT * 3,
                 cudaMemcpyDeviceToHost);
      image->saveFile("mandelbulb.ppm");
      delete image;
    }
  }

  cudaFree(d_buffer);
  std::cout << "All renderings are done\n";
  return 0;
}