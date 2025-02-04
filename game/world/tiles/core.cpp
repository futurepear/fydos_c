#include "core.h"

Tile* makeTile(int loc, int id, Vector<float> chunkPosition) {
	if (id == 7)
		return new CraftingTile{ loc, id, chunkPosition };
	if (id == 30)
		return new ChestTile{ loc, id, chunkPosition };
	return new Tile{ loc, id, chunkPosition };
}