import XCTest
@testable import MicrobenchLib

final class MicrobenchLibTests: XCTestCase {
    func testSayHello() {
        XCTAssertEqual(MicrobenchLib.sayHello(), "Hello from MicrobenchLib!")
    }
    func testSayHello2() {
        XCTAssertEqual(MicrobenchLib.sayHello(), "Hello from MicrobenchLib!2")
    }
}
