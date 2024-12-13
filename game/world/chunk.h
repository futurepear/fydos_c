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
	int baseX = 0;
	int baseY = 0;
public:
	Tile biome[Constants::chunkWidth * Constants::chunkWidth]{};
	Tile floor[Constants::chunkWidth * Constants::chunkWidth]{};
	Tile tiles[Constants::chunkWidth * Constants::chunkWidth]{};
	Tile roof[Constants::chunkWidth * Constants::chunkWidth]{};

	Chunk(int x = 0, int y = 0) : baseX{ x }, baseY{ y } {
		for (int i = 0; i < Constants::chunkWidth * Constants::chunkWidth; i++) {
			biome[i] = Tile{ i, 2 };
			tiles[i] = Tile{ i, 0 };
		}
	}
	Tile* getLayer(int layer) {
		if (layer == 0) return biome;
		if (layer == 1) return floor;
		if (layer == 2) return tiles;
		if (layer == 3) return roof;
		return tiles;
	}
	Tile& getTile(int layer, int index) {
		return getLayer(layer)[index];
	}
	Tile* operator[](int index) {
		return getLayer(index);
	}
	template <typename T> int positionToLocation(T x, T y) const {
		int x2 = (int)(x-baseX); int y2 = (int)(y-baseY);
		int location = Constants::chunkWidth * (y2) + (x2);
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
		return baseX + 1 * Constants::chunkWidth;
	}
	int topBorder() const {
		return baseY;
	}
	int bottomBorder() const {
		return baseY + 1 * Constants::chunkWidth;
	}
	static constexpr int chunkLength = Constants::chunkWidth * Constants::chunkWidth;
};