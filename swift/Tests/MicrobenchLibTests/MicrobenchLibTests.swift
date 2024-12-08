import XCTest
@testable import MicrobenchLib

final class MicrobenchLibTests: XCTestCase {
    func testSayHello() {
        XCTAssertEqual(MicrobenchLib.sayHello(), "Hello from MicrobenchLib!")
    }
}
