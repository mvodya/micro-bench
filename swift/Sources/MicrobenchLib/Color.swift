struct Color {
  // Color values
  public var r, g, b: UInt8

  // Create raw color by components
  public init(r: UInt8, g: UInt8, b: UInt8) {
    self.r = r
    self.g = g
    self.b = b
  }

  // Create color from HEX
  public init(hex: UInt32) {
    r = UInt8((hex >> 16) & 0xFF)
    g = UInt8((hex >> 8) & 0xFF)
    b = UInt8((hex) & 0xFF)
  }

  // Create color from vector
  public init(_ v: Vec3) {
    r = UInt8(min(max((v.x * 255.0).rounded(), 0), 255));
    g = UInt8(min(max((v.y * 255.0).rounded(), 0), 255));
    b = UInt8(min(max((v.z * 255.0).rounded(), 0), 255));
  }

  // Get vector from color
  public func toVec3() -> Vec3 {
    return Vec3(x: Double(r) / 255.0, y: Double(g) / 255.0, z: Double(b) / 255.0);
  }
}
