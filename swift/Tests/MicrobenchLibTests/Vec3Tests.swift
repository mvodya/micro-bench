import XCTest
@testable import MicrobenchLib

let TOLERANCE = 1e-5;

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
}