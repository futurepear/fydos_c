#include "chunk.h"
#include "tiles/core.h"
#include "../constants.h"
#include <stdint.h>
#include "../physics/vector.h"
#include <vector>

Chunk::Chunk(int x, int y) : baseX{ x }, baseY{ y } {
	for (int i = 0; i < Constants::chunkWidth * Constants::chunkWidth; i++) {
		biome[i] = new Tile{ i, 2, Vector<float>{(float)leftBorder(), (float)topBorder()}};
		floor[i] = new Tile{ i, 0, Vector<float>{(float)leftBorder(), (float)topBorder()} };
		tiles[i] = new Tile{ i, 0, Vector<float>{(float)leftBorder(), (float)topBorder()} };
		roof[i] = new Tile{ i, 0, Vector<float>{(float)leftBorder(), (float)topBorder()} };
	}
}
Tile** Chunk::getLayer(int layer) {
	if (layer == 0) return biome;
	if (layer == 1) return floor;
	if (layer == 2) return tiles;
	if (layer == 3) return roof;
	return tiles;
}
Tile* Chunk::getTile(int layer, int index) {
	return getLayer(layer)[index];
}
Tile** Chunk::operator[](int index) {
	return getLayer(index);
}

void Chunk::replaceTile(int layer, int index, int tileID) {
	delete getLayer(layer)[index];
	getLayer(layer)[index] = makeTile( index, tileID, Vector<float>{(float)leftBorder(), (float)topBorder()} );
}

int Chunk::leftBorder() {
	return baseX;
}
int Chunk::rightBorder() const {
	return baseX + 1 * Constants::chunkWidth;
}
int Chunk::topBorder() const {
	return baseY;
}
int Chunk::bottomBorder() const {
	return baseY + 1 * Constants::chunkWidth;
}