#pragma once
#include "tile.h"

class ChestTile : public Tile {
private:
	ItemBuffer* storage;
	void closeScreen();
public:
	ChestTile(int loc = 0, int id = 0, Vector<float> chunkPos = { 0, 0 });
	~ChestTile();
	void interact(TileInteractionEvent t);
	void clientSideUpdate(bool canInteract);
};
