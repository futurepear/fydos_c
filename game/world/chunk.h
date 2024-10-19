#pragma once
#include <stdint.h>
#include <vector>
#include <cmath>
#include "../constants.h"
#include "../physics/vector.h"
#include "tile.h"


class Player;

class Chunk {
private:
	Tile tiles[Constants::chunkSize * Constants::chunkSize]{};
	int baseX = 0;
	int baseY = 0;
public:
	Chunk(int x = 0, int y = 0) : baseX{ x }, baseY{ y } {
		for (int i = 0; i < Constants::chunkSize * Constants::chunkSize; i++) {
			tiles[i] = Tile{ i, 0 };
		}
	}
	Tile& getTile(int index) {
		return tiles[index];
	}
	Tile& operator[](int index) {
		return tiles[index];
	}
	int positionToLocation(int x, int y) const {
		x -= baseX;
		y -= baseY;
		int location = Constants::chunkSize * (y / 100) + (x/100);
		if (location < 0 || location >= Chunk::chunkLength) return -1;
		return location;
	}
	template <typename T> Vector<int> tileSpace(const Vector<T> vector) const {
		T x = (vector.x - baseX) / (Constants::tileWidth);
		T y = (vector.y - baseY) / (Constants::tileWidth);
		return Vector<int>{ (int)std::floor(x), (int)std::floor(y) };
	}
	int leftBorder() {
		return baseX;
	}
	int rightBorder() const {
		return baseX + 100 * Constants::chunkSize;
	}
	int topBorder() const {
		return baseY;
	}
	int bottomBorder() const {
		return baseY + 100 * Constants::chunkSize;
	}
	static constexpr int chunkLength = Constants::chunkSize * Constants::chunkSize;
};