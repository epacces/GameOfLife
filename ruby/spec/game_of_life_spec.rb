# spec/game_of_life_spec.rb
require_relative '../game'

RSpec.describe GameOfLife do
  let(:game) { GameOfLife.new(3, 3) }
  let(:alive) { GameOfLife::ALIVE }
  let(:dead)  { GameOfLife::DEAD }

  describe 'cell storage' do
    it 'lets you set and get a cell via private helpers' do
      p = Position.new(1, 2)
      game.send(:set_cell, alive, p)
      expect(game.send(:get_cell, p)).to eq(alive)
    end
  end

  describe '#neighbors' do
    context 'in the center of a 3×3 board' do
      it 'returns exactly 8 neighbors' do
        center = Position.new(1, 1)
        nbs = game.send(:neighbors, center)
        expect(nbs.size).to eq(8)
        # ensure all are within bounds
        expect(nbs).to all(satisfy { |p|
          p.x.between?(0,2) && p.y.between?(0,2)
        })
      end
    end

    context 'in a corner of a 3×3 board' do
      it 'returns only the neighbors that exist (no wrapping)' do
        corner = Position.new(0, 0)
        nbs = game.send(:neighbors, corner)
        # should be positions (1,0), (0,1), (1,1)
        coords = nbs.map { |p| [p.x,p.y] }
        expect(coords.sort).to match_array([
          [-1,-1],[-1,0],[-1,1],[0,-1],[0,1],[1,-1],[1,0],[1,1]
        ])
      end
    end
  end

  describe 'Game of Life rules via #next_state' do
    def set_alive(*coords)
      coords.each do |x,y|
        game.send(:set_cell, alive, Position.new(x,y))
      end
    end

    def cell_at(x,y)
      game.send(:get_cell, Position.new(x,y))
    end

    it 'evolves a “blinker” oscillator correctly' do
      # vertical blinker at (1,0),(1,1),(1,2)
      set_alive([1,0], [1,1], [1,2])
      game.send(:next_state)
      # should become horizontal at y=1
      expect(cell_at(0,1)).to eq(alive)
      expect(cell_at(1,1)).to eq(alive)
      expect(cell_at(2,1)).to eq(alive)
      # other cells should be dead
      expect(cell_at(1,0)).to eq(dead)
      expect(cell_at(1,2)).to eq(dead)
    end

    it 'preserves a 2×2 block (still life)' do
      # block at (0,0),(0,1),(1,0),(1,1)
      set_alive([0,0], [0,1], [1,0], [1,1])
      before = (0..1).flat_map { |y| (0..1).map { |x| cell_at(x,y) } }
      game.send(:next_state)
      after = (0..1).flat_map { |y| (0..1).map { |x| cell_at(x,y) } }
      expect(after).to eq(before)
    end
  end
end
