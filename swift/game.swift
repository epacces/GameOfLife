import Foundation

final class GameOfLife {
    enum CellState: Character {
        case alive = "*"
        case dead = "."
    }

    struct Position {
        let x: Int
        let y: Int

        var isZero: Bool {
            x == 0 && y == 0
        }

        static func +(lhs: Self, rhs: Self) -> Self {
            .init(x: lhs.x + rhs.x, y: lhs.y + rhs.y)
        }
    }

    private let rows: Int
    private let cols: Int
    private var board: [CellState]
    private var buffer: [CellState]

    init(rows: Int = 20, cols: Int = 90) {
        self.rows = rows
        self.cols = cols
        self.board = Array(repeating: CellState.dead, count: rows*cols)
        self.buffer = Array(repeating: CellState.dead, count: rows*cols)
    }

    private func index(from position: Position) -> Int {
        let x = (position.x%cols + cols)%cols
        let y = (position.y%rows + rows)%rows
        return y*cols+x
    }

    func drawBoard() {
        clearScreen()
        for y in 0..<rows {
            for x in 0..<cols {
                let index = index(from: Position(x: x, y: y))
                print("\(board[index].rawValue)", terminator:"")
            }
            print("")
        }
    }

    private func countAliveNeighbors(at position: Position) -> Int {
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

    private func setCell(state: CellState, at position: Position) {
        board[index(from: position)] = state;
    }

    private func getCell(at position: Position) -> CellState {
        board[index(from: position)]
    }

    private func nextState() {
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
        // ANSI escape: clear screen and move cursor to home
        print("\u{1B}[2J\u{1B}[H", terminator: "")
    }

    private func randomizeState() {
        for y in 0..<rows {
            for x in 0..<cols {
                if Int.random(in: 0...1) == 1 {
                    setCell(state: .alive, at: .init(x: x, y: y))
                }
            }
        }
    }

    func run() {
        randomizeState()
        while true {
            drawBoard()
            nextState()
            Thread.sleep(forTimeInterval:0.050)
        }
    }
}

func main() {
    let game = GameOfLife()
    game.run()
}


main()
