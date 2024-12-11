import XCTest

@testable import MicrobenchLib

final class ColorTests: XCTestCase {
  func testConstructor() {
    let color = Color(r: 10, g: 23, b: 255)

    XCTAssertEqual(color.r, 10)
    XCTAssertEqual(color.g, 23)
    XCTAssertEqual(color.b, 255)
  }

  func testHexConstructor() {
    let color = Color(hex: 0xFF1232)

    XCTAssertEqual(color.r, 255)
    XCTAssertEqual(color.g, 18)
    XCTAssertEqual(color.b, 50)
  }

  func testVec3Constructor() {
    let color = Color(Vec3(x: 0.1, y: 0.5, z: 1.0))

    XCTAssertEqual(color.r, 26)
    XCTAssertEqual(color.g, 128)
    XCTAssertEqual(color.b, 255)
  }

  func testConvertToVec3() {
    let color = Color(r: 26, g: 128, b: 255)
    let v = color.toVec3();

    XCTAssertEqual(v.x, 0.1, accuracy: TOLERANCE)
    XCTAssertEqual(v.y, 0.5, accuracy: TOLERANCE)
    XCTAssertEqual(v.z, 1.0, accuracy: TOLERANCE)
  }
}
