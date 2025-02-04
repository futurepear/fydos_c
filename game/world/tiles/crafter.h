#pragma once
#include "tile.h"

class CraftingTile : public Tile {
private:
	ItemBuffer* input;
	ItemBuffer* output;
	Crafter* crafter;
	void closeScreen();
public:
	CraftingTile(int loc = 0, int id = 0, Vector<float> chunkPos = { 0, 0 });
	~CraftingTile();
	void interact(TileInteractionEvent t);
	void update();
	void clientSideUpdate(bool canInteract);
};
