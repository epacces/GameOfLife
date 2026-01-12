# spec/position_spec.rb
require_relative '../game'

RSpec.describe Position do
  describe '#+' do
    it 'returns a new Position with component-wise sums' do
      a = Position.new(2, 3)
      b = Position.new(5, -1)
      sum = a + b
      expect(sum).to be_a(Position)
      expect(sum.x).to eq(7)
      expect(sum.y).to eq(2)
    end

    it 'does not modify either operand' do
      a = Position.new(1, 1)
      b = Position.new(2, 2)
      _ = a + b
      expect(a.x).to eq(1)
      expect(a.y).to eq(1)
      expect(b.x).to eq(2)
      expect(b.y).to eq(2)
    end
  end

  describe '#zero?' do
    it 'returns true if both coordinates are zero' do
      expect(Position.new(0, 0).zero?).to be true
    end

    it 'returns false otherwise' do
      expect(Position.new(0, 1).zero?).to be false
      expect(Position.new(1, 0).zero?).to be false
      expect(Position.new(1, 1).zero?).to be false
    end
  end
end
