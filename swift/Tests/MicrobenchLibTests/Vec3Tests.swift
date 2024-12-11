import XCTest

@testable import MicrobenchLib

final class Vec3Tests: XCTestCase {
  func testSummation() {
    let v1 = Vec3(x: 1.0, y: 2.0, z: 3.0)
    let v2 = Vec3(x: 4.0, y: 5.0, z: 6.0)
    let result = v1 + v2

    XCTAssertEqual(result.x, 5, accuracy: TOLERANCE)
    XCTAssertEqual(result.y, 7, accuracy: TOLERANCE)
    XCTAssertEqual(result.z, 9, accuracy: TOLERANCE)
  }

  func testSubstraction() {
    let v1 = Vec3(x: 4.0, y: 5.0, z: 7.0)
    let v2 = Vec3(x: 5.0, y: 2.0, z: 3.0)
    let result = v1 - v2

    XCTAssertEqual(result.x, -1.0, accuracy: TOLERANCE)
    XCTAssertEqual(result.y, 3.0, accuracy: TOLERANCE)
    XCTAssertEqual(result.z, 4.0, accuracy: TOLERANCE)
  }

  func testMultiplication() {
    let v = Vec3(x: 1.0, y: 2.0, z: 3.0)
    let result = v * 2.0

    XCTAssertEqual(result.x, 2.0, accuracy: TOLERANCE)
    XCTAssertEqual(result.y, 4.0, accuracy: TOLERANCE)
    XCTAssertEqual(result.z, 6.0, accuracy: TOLERANCE)
  }

  func testScalarMultiplication() {
    let v1 = Vec3(x: 1.0, y: 2.0, z: 3.0)
    let v2 = Vec3(x: 4.0, y: -5.0, z: 6.0)
    let result = v1 * v2
    XCTAssertEqual(result, 12.0, accuracy: TOLERANCE)
  }

  func testLength() {
    let v = Vec3(x: 3.0, y: 0.0, z: 4.0)
    let result = v.length();
    XCTAssertEqual(result, 5.0, accuracy: TOLERANCE)
  }

  func testNormalizztion() {
    let v = Vec3(x: 3.0, y: 0.0, z: 4.0)
    let result = v.normalize();
    XCTAssertEqual(result.x, 0.6, accuracy: TOLERANCE)
    XCTAssertEqual(result.y, 0.0, accuracy: TOLERANCE)
    XCTAssertEqual(result.z, 0.8, accuracy: TOLERANCE)
    XCTAssertEqual(result.length(), 1.0, accuracy: TOLERANCE)
  }

  func testConvertToColor() {
    let v = Vec3(x: 3.0, y: 1.0, z: 0.5)
    let color = v.toColor()

    XCTAssertEqual(color.r, 255)
    XCTAssertEqual(color.g, 255)
    XCTAssertEqual(color.b, 128)
  }
}
