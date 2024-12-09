import Foundation

struct Vec3 {
  // Vector values
  public var x, y, z: Double

  public init(x: Double, y: Double, z: Double) {
    self.x = x
    self.y = y
    self.z = z
  }

  // Summation
  static func + (a: Vec3, b: Vec3) -> Vec3 {
    return Vec3(x: a.x + b.x, y: a.y + b.y, z: a.z + b.z)
  }
  // Subtraction
  static func - (a: Vec3, b: Vec3) -> Vec3 {
    return Vec3(x: a.x - b.x, y: a.y - b.y, z: a.z - b.z)
  }
  // Multiplication by value
  static func * (a: Vec3, f: Double) -> Vec3 {
    return Vec3(x: a.x * f, y: a.y * f, z: a.z * f)
  }
  // Scalar multiplication
  static func * (a: Vec3, b: Vec3) -> Double {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
  }
  // Calculate vector length
  public func length() -> Double {
    return (x * x + y * y + z * z).squareRoot();
  }

  // Get vector normalize
  public func normalize() -> Vec3 {
    let len = length();
    return Vec3(x: x / len, y: y / len, z: z / len);
  }

  // Convert to color
  //TODO
}
