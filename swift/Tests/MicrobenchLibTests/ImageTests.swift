import XCTest

@testable import MicrobenchLib

class ImageTests: XCTestCase {
  func testImageCreation() {
    let image = Image(width: 10, height: 20)
    XCTAssertEqual(image.width, 10)
    XCTAssertEqual(image.height, 20)
  }

  func testDefaultPixelAccess() {
    let image = Image(width: 10, height: 20, defaultColor: Color(hex: UInt32(0xFF11EE)))

    // Foreach image pixels and check color
    for y in 0..<image.height {
      for x in 0..<image.width {
        let expectedColor = Color(hex: UInt32(0xFF11EE))
        XCTAssertEqual(image[x, y], expectedColor)
      }
    }
  }

  func testPixelPosition() {
    var image = Image(width: 40, height: 50)

    // Set pixel color based on position
    for y in 0..<image.height {
      for x in 0..<image.width {
        image[x, y] = Color(r: UInt8(x), g: UInt8(y), b: 0)
      }
    }

    // Check pixel color based on position
    for y in 0..<image.height {
      for x in 0..<image.width {
        XCTAssertEqual(image[x, y], Color(r: UInt8(x), g: UInt8(y), b: 0))
      }
    }
  }
}
