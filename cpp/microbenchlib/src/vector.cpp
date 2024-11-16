#include <cmath>

#include "microbench.hpp"

using namespace MicroBench;

Vec3 Vec3::operator+(const Vec3& v) const {
  return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v) const {
  return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator*(double f) const { return Vec3(x * f, y * f, z * f); }

double Vec3::operator*(const Vec3& v) const {
  return x * v.x + y * v.y + z * v.z;
}

double Vec3::length() const { return std::sqrt(x * x + y * y + z * z); }

Vec3 Vec3::normalize() const {
  double len = length();
  return Vec3(x / len, y / len, z / len);
}

Vec3::operator Color() const {
  return Color((uint8_t)(x * 255.0), (uint8_t)(y * 255.0),
               (uint8_t)(z * 255.0));
}