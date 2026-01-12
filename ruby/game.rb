#!/usr/bin/env ruby

# A simple 2D position with + operator
class Position
  attr_reader :x, :y

  def initialize(x, y)
    @x, @y = x, y
  end

  def +(other)
    self.class.new(x + other.x, y + other.y)
  end

  def zero?
    x.zero? && y.zero?
  end
end

class GameOfLife
  ALIVE = "*"
  DEAD  = "."

  def initialize(rows = 20, cols = 90)
    @rows, @cols = rows, cols
    @board  = Array.new(rows) { Array.new(cols, DEAD) }
    @buffer = Array.new(rows) { Array.new(cols, DEAD) }
  end

  def run
    randomize_state
    loop do
      clear_screen
      draw_board
      next_state
      sleep 0.050
    end
  end

  private

  # ANSI clear + home
  def clear_screen
    print "\e[2J\e[H"
  end

  def randomize_state
    each_position do |pos|
      set_cell(ALIVE, pos) if rand(2).zero?
    end
  end

  def next_state
    each_position do |pos|
      live_n = neighbors(pos).count { |n| 
        get_cell(n) == ALIVE 
      }
      current = get_cell(pos)

      new_state =
        if current == ALIVE && (live_n == 2 || live_n == 3)
          ALIVE
        elsif current == DEAD && live_n == 3
          ALIVE
        else
          DEAD
        end

      @buffer[pos.y][pos.x] = new_state
    end

    # swap buffers
    @board, @buffer = @buffer, @board
  end

  def draw_board
    @rows.times do |y|
      @cols.times do |x|
        print get_cell(Position.new(x, y))
      end
      puts
    end
  end

  # ==== Helpers ====

  def get_cell(pos)
    @board[pos.y % @rows][pos.x % @cols]
  end

  def set_cell(state, pos)
    @board[pos.y % @rows][pos.x % @cols] = state
  end

  def each_position
    @rows.times do |y|
      @cols.times do |x|
        yield Position.new(x, y)
      end
    end
  end

  def neighbors(pos)
    deltas = [
      [-1,-1],[0,-1],[1,-1],
      [-1, 0],      [1, 0],
      [-1, 1],[0, 1],[1, 1],
    ]
    deltas
      .map { |dx, dy| pos + Position.new(dx, dy) }
  end
end

if __FILE__ == $0
  GameOfLife.new.run
end
