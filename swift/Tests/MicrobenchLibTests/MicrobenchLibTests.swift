import XCTest
@testable import MicrobenchLib

let TOLERANCE = 0.01

final class MicrobenchLibTests: XCTestCase {
    func testSayHello() {
        XCTAssertEqual(MicrobenchLib.sayHello(), "Hello from MicrobenchLib!")
    }
}
