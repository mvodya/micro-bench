// swift-tools-version: 6.0

import PackageDescription

let package = Package(
    name: "Microbench",
    products: [
        .library(name: "MicrobenchLib", targets: ["MicrobenchLib"]),
        .executable(name: "MandelbulbSinglethread", targets: ["MandelbulbSinglethread"]),
    ],
    targets: [
        .target(
            name: "MicrobenchLib",
            dependencies: []
        ),
        .testTarget(
            name: "MicrobenchLibTests",
            dependencies: ["MicrobenchLib"],
            swiftSettings: [
                .unsafeFlags(["-Xcc", "-D__STDC_NO_ATOMICS__"])
            ]
        ),
        .executableTarget(
            name: "MandelbulbSinglethread",
            dependencies: ["MicrobenchLib"]
        )
    ]
)
