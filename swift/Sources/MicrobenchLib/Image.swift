typealias Dimension = UInt

struct Image {
  // Image dimensions
  let width, height: Dimension

  // Pixels
  //  %--- width / x --→
  //  |                *
  //  |                *
  //  height           *
  //  y                *
  //  |                *
  //  ↓ *  *  *  *  *  *
  //
  private(set) var pixels: [Color]

  init(width: Dimension, height: Dimension, defaultColor: Color = Color(r: 0, g: 0, b: 0)) {
    self.width = width
    self.height = height
    // Initialize the image with default colors at each pixel location
    self.pixels = Array(repeating: defaultColor, count: Int(width * height))
  }

  // Access or update color at a specific pixel location (x, y) in the image
  subscript(x: Dimension, y: Dimension) -> Color {
    get {
      let index = Int(y) * Int(width) + Int(x)
      // Check bounds before accessing the pixel color
      guard x >= 0 && x < width,
            y >= 0 && y < height else {
        fatalError("Invalid pixel coordinates. Index out of bounds")
      }
      return pixels[index]
    }
    set {
      let index = Int(y) * Int(width) + Int(x)
      // Check bounds before updating the pixel color
      guard x >= 0 && x < width,
            y >= 0 && y < height else {
        fatalError("Invalid pixel coordinates. Index out of bounds")
      }
      pixels[index] = newValue
    }
  }
}
