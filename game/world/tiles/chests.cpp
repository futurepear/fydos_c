#include "chests.h"

ChestTile::ChestTile(int loc, int id, Vector<float> chunkPos) : Tile{ loc, id, chunkPos } {
	storage = new ItemStorage{ 100.0f };
	storage->setPhysicalPosition(&position);
}
ChestTile::~ChestTile() {
	closeScreen();
	delete storage;
}

void ChestTile::clientSideUpdate(bool canInteract) {
	GUIElement* screen = GUI::$(uuid());
	if (!canInteract && screen != nullptr) closeScreen();
}

void ChestTile::closeScreen() {
	GUIElement* screen = GUI::$(uuid());
	if (screen != nullptr)
		GUI::deleteElement(screen);
}

void ChestTile::interact(TileInteractionEvent t) {
	GUIElement* screen = GUI::$(uuid());

	if (screen != nullptr) {
		closeScreen();
		return;
	}

	screen = Components::inventory(storage);
	screen->setWidth(300, 'p');
	screen->setHeight(300, 'p');
	screen->setTop(100, 'p');
	screen->setLeft(100, 'p');
	GUI::setID(screen, uuid());
	GUI::root.appendChild(screen);
}
