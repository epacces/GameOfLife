// swift-tools-version:5.5
import PackageDescription

let package = Package(
    name: "GameOfLife",
    products: [
    .executable(name: "GameOfLife", targets: ["GameOfLife"])
    ],
    targets: [
        .target(
            name: "GameOfLife",
            path: "Sources"
        ),
        .testTarget(
            name: "GameOfLifeTests",
            dependencies: ["GameOfLife"],
            path: "Tests"
        )
    ]
)
