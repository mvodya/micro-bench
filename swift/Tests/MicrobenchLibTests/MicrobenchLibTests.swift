import XCTest
@testable import MicrobenchLib

let TOLERANCE = 1e-5

final class MicrobenchLibTests: XCTestCase {
    func testSayHello() {
        XCTAssertEqual(MicrobenchLib.sayHello(), "Hello from MicrobenchLib!")
    }
}
