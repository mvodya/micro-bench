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
  //TODO
  // Calculate vector length
  //TODO
  // Get vector normalize
  //TODO

  // Convert to color
  //TODO
}
