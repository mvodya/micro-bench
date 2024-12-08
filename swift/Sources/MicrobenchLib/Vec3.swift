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
  //TODO
  // Multiplication by value
  //TODO
  // Scalar multiplication
  //TODO
  // Calculate vector length
  //TODO
  // Get vector normalize
  //TODO

  // Convert to color
  //TODO
}
