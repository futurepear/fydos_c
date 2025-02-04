#pragma once
#include <stdint.h>
#include <vector>
#include <cmath>
#include "../constants.h"
#include "../physics/vector.h"
#include "tiles/core.h"


class Player;

class Chunk {
private:
	int baseX = 0;
	int baseY = 0;
public:
	Tile* biome[Constants::chunkLayerSize]{};
	Tile* floor[Constants::chunkLayerSize]{};
	Tile* tiles[Constants::chunkLayerSize]{};
	Tile* roof[Constants::chunkLayerSize]{};

	Chunk(int x = 0, int y = 0);
	Tile** getLayer(int layer);
	Tile* getTile(int layer, int index);
	Tile** operator[](int index);
	void replaceTile(int layer, int index, int tileID);
	int leftBorder();
	int rightBorder() const;
	int topBorder() const;
	int bottomBorder() const;

	template <typename T> int positionToLocation(T x, T y) const {
		int x2 = (int)(x - baseX); int y2 = (int)(y - baseY);
		int location = Constants::chunkWidth * (y2)+(x2);
		if (location < 0 || location >= Chunk::chunkLength) return -1;
		return location;
	}
	template <typename T> Vector<int> tileSpace(const Vector<T> vector) const {
		T x = (vector.x - baseX); 
		T y = (vector.y - baseY); 
		return Vector<int>{ (int)std::floor(x), (int)std::floor(y) };
	}

	static constexpr int chunkLength = Constants::chunkWidth * Constants::chunkWidth;
};