import Foundation

public final class GameOfLife {
    public enum CellState: Character {
        case alive = "*"
        case dead = "."
    }

    public struct Position {
        public let x: Int
        public let y: Int

        public init(x: Int, y: Int) {
            self.x = x
            self.y = y
        }

        public var isZero: Bool {
            x == 0 && y == 0
        }

        public static func +(lhs: Self, rhs: Self) -> Self {
            .init(x: lhs.x + rhs.x, y: lhs.y + rhs.y)
        }
    }

    public let rows: Int
    public let cols: Int
    internal var board: [CellState]
    internal var buffer: [CellState]

    public init(rows: Int = 20, cols: Int = 90) {
        self.rows = rows
        self.cols = cols
        self.board = Array(repeating: CellState.dead, count: rows*cols)
        self.buffer = Array(repeating: CellState.dead, count: rows*cols)
    }

    internal func index(from position: Position) -> Int {
        let x = (position.x%cols + cols)%cols
        let y = (position.y%rows + rows)%rows
        return y*cols+x
    }

    public func drawBoard() {
        clearScreen()
        for y in 0..<rows {
            for x in 0..<cols {
                let index = index(from: Position(x: x, y: y))
                print("\(board[index].rawValue)", terminator:"")
            }
            print("")
        }
    }

    internal func countAliveNeighbors(at position: Position) -> Int {
        var aliveCount = 0
        for yOff in -1...1 {
            for xOff in -1...1 {
                let offset = Position(x: xOff, y: yOff)
                if offset.isZero { continue }
                aliveCount += getCell(at: position + offset) == .alive ? 1 : 0
            }
        }
        return aliveCount
    }

    internal func setCell(state: CellState, at position: Position) {
        board[index(from: position)] = state;
    }

    internal func getCell(at position: Position) -> CellState {
        board[index(from: position)]
    }

    internal func nextState() {
        for y in 0..<rows {
            for x in 0..<cols {
                let position = Position(x: x, y: y)
                let count = countAliveNeighbors(at: position)
                var next: CellState = .dead
                if count == 3 || (getCell(at: position) == .alive && count == 2) {
                    next = .alive
                }
                buffer[index(from: position)] = next
            }
        }
        board = buffer
    }

    private func clearScreen() {
        print("\u{1B}[2J\u{1B}[H", terminator: "")
    }

    internal func randomizeState() {
        for y in 0..<rows {
            for x in 0..<cols {
                if Int.random(in: 0...1) == 1 {
                    setCell(state: .alive, at: .init(x: x, y: y))
                }
            }
        }
    }

    public func run() {
        randomizeState()
        while true {
            drawBoard()
            nextState()
            Thread.sleep(forTimeInterval:0.050)
        }
    }
}
