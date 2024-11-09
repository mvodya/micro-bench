#include "microbench.hpp"

using namespace MicroBench;

Vec3 Vec3::operator+(const Vec3& v) const {
  return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v) const {
  return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator*(float f) const { return Vec3(x * f, y * f, z * f); }

float Vec3::length() const { return std::sqrt(x * x + y * y + z * z); }

Vec3 Vec3::normalized() const {
  float len = length();
  return Vec3(x / len, y / len, z / len);
}