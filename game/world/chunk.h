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

	Tile biome[Constants::chunkSize * Constants::chunkSize]{};
	Tile floor[Constants::chunkSize * Constants::chunkSize]{};
	Tile tiles[Constants::chunkSize * Constants::chunkSize]{};
	Tile roof[Constants::chunkSize * Constants::chunkSize]{};

	int baseX = 0;
	int baseY = 0;
public:
	Chunk(int x = 0, int y = 0) : baseX{ x }, baseY{ y } {
		for (int i = 0; i < Constants::chunkSize * Constants::chunkSize; i++) {
			tiles[i] = Tile{ i, 0 };
		}
	}
	Tile* getLayer(int layer) {
		if (layer == 0) return biome;
		if (layer == 1) return floor;
		if (layer == 2) return tiles;
		if (layer == 3) return roof;
	}
	Tile& getTile(int layer, int index) {
		return getLayer(layer)[index];
	}
	Tile* operator[](int index) {
		return getLayer(index);
	}
	template <typename T> int positionToLocation(T x, T y) const {
		int x2 = (int)(x-baseX); int y2 = (int)(y-baseY);
		int location = Constants::chunkSize * (y2) + (x2);
		if (location < 0 || location >= Chunk::chunkLength) return -1;
		return location;
	}
	template <typename T> Vector<int> tileSpace(const Vector<T> vector) const {
		T x = (vector.x - baseX); // (Constants::tileWidth);
		T y = (vector.y - baseY); // (Constants::tileWidth);
		return Vector<int>{ (int)std::floor(x), (int)std::floor(y) };
	}
	int leftBorder() {
		return baseX;
	}
	int rightBorder() const {
		return baseX + 1 * Constants::chunkSize;
	}
	int topBorder() const {
		return baseY;
	}
	int bottomBorder() const {
		return baseY + 1 * Constants::chunkSize;
	}
	static constexpr int chunkLength = Constants::chunkSize * Constants::chunkSize;
};