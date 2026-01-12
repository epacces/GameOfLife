import XCTest
@testable import GameOfLife

final class PositionTests: XCTestCase {

    func testAddition() {
        let a = GameOfLife.Position(x: 2, y: 3)
        let b = GameOfLife.Position(x: 5, y: -1)
        let sum = a + b

        XCTAssertEqual(sum.x, 7)
        XCTAssertEqual(sum.y, 2)
    }

    func testAdditionDoesNotModifyOperands() {
        let a = GameOfLife.Position(x: 1, y: 1)
        let b = GameOfLife.Position(x: 2, y: 2)
        _ = a + b

        XCTAssertEqual(a.x, 1)
        XCTAssertEqual(a.y, 1)
        XCTAssertEqual(b.x, 2)
        XCTAssertEqual(b.y, 2)
    }

    func testIsZeroTrue() {
        XCTAssertTrue(GameOfLife.Position(x: 0, y: 0).isZero)
    }

    func testIsZeroFalse() {
        XCTAssertFalse(GameOfLife.Position(x: 0, y: 1).isZero)
        XCTAssertFalse(GameOfLife.Position(x: 1, y: 0).isZero)
        XCTAssertFalse(GameOfLife.Position(x: 1, y: 1).isZero)
    }
}

final class IndexTests: XCTestCase {
    let game = GameOfLife(rows: 5, cols: 5)

    func testBasicIndex() {
        XCTAssertEqual(game.index(from: .init(x: 0, y: 0)), 0)
        XCTAssertEqual(game.index(from: .init(x: 1, y: 0)), 1)
        XCTAssertEqual(game.index(from: .init(x: 0, y: 1)), 5)
        XCTAssertEqual(game.index(from: .init(x: 2, y: 2)), 12)
    }

    func testPositiveWrapping() {
        // x wraps around
        XCTAssertEqual(
            game.index(from: .init(x: 5, y: 0)),
            game.index(from: .init(x: 0, y: 0))
        )
        XCTAssertEqual(
            game.index(from: .init(x: 6, y: 0)),
            game.index(from: .init(x: 1, y: 0))
        )
        // y wraps around
        XCTAssertEqual(
            game.index(from: .init(x: 0, y: 5)),
            game.index(from: .init(x: 0, y: 0))
        )
        XCTAssertEqual(
            game.index(from: .init(x: 0, y: 6)),
            game.index(from: .init(x: 0, y: 1))
        )
    }

    func testNegativeWrapping() {
        // negative x wraps
        XCTAssertEqual(
            game.index(from: .init(x: -1, y: 0)),
            game.index(from: .init(x: 4, y: 0))
        )
        XCTAssertEqual(
            game.index(from: .init(x: -2, y: 0)),
            game.index(from: .init(x: 3, y: 0))
        )
        // negative y wraps
        XCTAssertEqual(
            game.index(from: .init(x: 0, y: -1)),
            game.index(from: .init(x: 0, y: 4))
        )
        XCTAssertEqual(
            game.index(from: .init(x: 0, y: -2)),
            game.index(from: .init(x: 0, y: 3))
        )
    }
}

final class CellTests: XCTestCase {

    func testSetAndGetCell() {
        let game = GameOfLife(rows: 5, cols: 5)

        game.setCell(state: .alive, at: .init(x: 1, y: 2))
        XCTAssertEqual(game.getCell(at: .init(x: 1, y: 2)), .alive)
        XCTAssertEqual(game.getCell(at: .init(x: 0, y: 0)), .dead)
    }

    func testSetCellWithWrapping() {
        let game = GameOfLife(rows: 5, cols: 5)

        // Set cell using wrapped coordinates
        game.setCell(state: .alive, at: .init(x: -1, y: -1))
        XCTAssertEqual(game.getCell(at: .init(x: 4, y: 4)), .alive)

        game.setCell(state: .alive, at: .init(x: 5, y: 5))
        XCTAssertEqual(game.getCell(at: .init(x: 0, y: 0)), .alive)
    }
}

final class NeighborTests: XCTestCase {

    func testNoNeighbors() {
        let game = GameOfLife(rows: 5, cols: 5)
        XCTAssertEqual(game.countAliveNeighbors(at: .init(x: 2, y: 2)), 0)
    }

    func testAllEightNeighbors() {
        let game = GameOfLife(rows: 5, cols: 5)

        // Surround center cell (2,2) with alive cells
        for dy in -1...1 {
            for dx in -1...1 {
                if dx != 0 || dy != 0 {
                    game.setCell(state: .alive, at: .init(x: 2 + dx, y: 2 + dy))
                }
            }
        }
        XCTAssertEqual(game.countAliveNeighbors(at: .init(x: 2, y: 2)), 8)
    }

    func testDoesNotCountSelf() {
        let game = GameOfLife(rows: 5, cols: 5)

        // Only the cell itself is alive
        game.setCell(state: .alive, at: .init(x: 2, y: 2))
        XCTAssertEqual(game.countAliveNeighbors(at: .init(x: 2, y: 2)), 0)
    }

    func testNeighborsWithWrapping() {
        let game = GameOfLife(rows: 5, cols: 5)

        // Cell at corner (0,0), place alive cells at wrapped positions
        game.setCell(state: .alive, at: .init(x: 4, y: 4))  // wraps to top-left diagonal
        game.setCell(state: .alive, at: .init(x: 1, y: 0))
        game.setCell(state: .alive, at: .init(x: 0, y: 1))

        XCTAssertEqual(game.countAliveNeighbors(at: .init(x: 0, y: 0)), 3)
    }
}

final class GameOfLifeRulesTests: XCTestCase {

    func testBlinkerOscillator() {
        let game = GameOfLife(rows: 5, cols: 5)

        // Vertical blinker at column 2: (2,1), (2,2), (2,3)
        game.setCell(state: .alive, at: .init(x: 2, y: 1))
        game.setCell(state: .alive, at: .init(x: 2, y: 2))
        game.setCell(state: .alive, at: .init(x: 2, y: 3))

        game.nextState()

        // Should become horizontal at row 2: (1,2), (2,2), (3,2)
        XCTAssertEqual(game.getCell(at: .init(x: 1, y: 2)), .alive)
        XCTAssertEqual(game.getCell(at: .init(x: 2, y: 2)), .alive)
        XCTAssertEqual(game.getCell(at: .init(x: 3, y: 2)), .alive)

        // Original vertical cells (except center) should be dead
        XCTAssertEqual(game.getCell(at: .init(x: 2, y: 1)), .dead)
        XCTAssertEqual(game.getCell(at: .init(x: 2, y: 3)), .dead)
    }

    func testBlockStillLife() {
        let game = GameOfLife(rows: 5, cols: 5)

        // 2x2 block at (1,1), (1,2), (2,1), (2,2)
        game.setCell(state: .alive, at: .init(x: 1, y: 1))
        game.setCell(state: .alive, at: .init(x: 1, y: 2))
        game.setCell(state: .alive, at: .init(x: 2, y: 1))
        game.setCell(state: .alive, at: .init(x: 2, y: 2))

        game.nextState()

        // Block should remain unchanged
        XCTAssertEqual(game.getCell(at: .init(x: 1, y: 1)), .alive)
        XCTAssertEqual(game.getCell(at: .init(x: 1, y: 2)), .alive)
        XCTAssertEqual(game.getCell(at: .init(x: 2, y: 1)), .alive)
        XCTAssertEqual(game.getCell(at: .init(x: 2, y: 2)), .alive)

        // Surrounding cells should still be dead
        XCTAssertEqual(game.getCell(at: .init(x: 0, y: 0)), .dead)
        XCTAssertEqual(game.getCell(at: .init(x: 0, y: 1)), .dead)
        XCTAssertEqual(game.getCell(at: .init(x: 3, y: 3)), .dead)
    }

    func testUnderpopulation() {
        let game = GameOfLife(rows: 5, cols: 5)

        // Single cell dies (0 neighbors)
        game.setCell(state: .alive, at: .init(x: 2, y: 2))
        game.nextState()
        XCTAssertEqual(game.getCell(at: .init(x: 2, y: 2)), .dead)
    }

    func testOverpopulation() {
        let game = GameOfLife(rows: 5, cols: 5)

        // Center cell with 4+ neighbors dies
        game.setCell(state: .alive, at: .init(x: 2, y: 2))
        game.setCell(state: .alive, at: .init(x: 1, y: 1))
        game.setCell(state: .alive, at: .init(x: 1, y: 2))
        game.setCell(state: .alive, at: .init(x: 1, y: 3))
        game.setCell(state: .alive, at: .init(x: 2, y: 1))

        game.nextState()
        XCTAssertEqual(game.getCell(at: .init(x: 2, y: 2)), .dead)
    }

    func testReproduction() {
        let game = GameOfLife(rows: 5, cols: 5)

        // Dead cell with exactly 3 neighbors becomes alive
        game.setCell(state: .alive, at: .init(x: 1, y: 2))
        game.setCell(state: .alive, at: .init(x: 2, y: 1))
        game.setCell(state: .alive, at: .init(x: 3, y: 2))

        game.nextState()
        XCTAssertEqual(game.getCell(at: .init(x: 2, y: 2)), .alive)
    }

    func testSurvivalWithTwoNeighbors() {
        let game = GameOfLife(rows: 5, cols: 5)

        // Live cell with exactly 2 neighbors survives
        game.setCell(state: .alive, at: .init(x: 2, y: 2))
        game.setCell(state: .alive, at: .init(x: 1, y: 2))
        game.setCell(state: .alive, at: .init(x: 3, y: 2))

        game.nextState()
        XCTAssertEqual(game.getCell(at: .init(x: 2, y: 2)), .alive)
    }

    func testSurvivalWithThreeNeighbors() {
        let game = GameOfLife(rows: 5, cols: 5)

        // Live cell with exactly 3 neighbors survives
        game.setCell(state: .alive, at: .init(x: 2, y: 2))
        game.setCell(state: .alive, at: .init(x: 1, y: 2))
        game.setCell(state: .alive, at: .init(x: 3, y: 2))
        game.setCell(state: .alive, at: .init(x: 2, y: 1))

        game.nextState()
        XCTAssertEqual(game.getCell(at: .init(x: 2, y: 2)), .alive)
    }
}
