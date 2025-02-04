#include "crafter.h"

CraftingTile::CraftingTile(int loc, int id, Vector<float> chunkPos) : Tile{ loc, id, chunkPos } {
	input = new ItemStorageOrdered{ 100, 5 };
	output = new ItemStorageOrdered{ 100, 5 };
	input->setPhysicalPosition(&position);
	output->setPhysicalPosition(&position);
	crafter = new Crafter{ input, output, 1 };
}

CraftingTile::~CraftingTile() {
	closeScreen();
	delete input;
	delete output;
	delete crafter;
}

void CraftingTile::closeScreen() {
	GUIElement* screen = GUI::$(uuid());
	if (screen != nullptr)
		GUI::deleteElement(screen);
	screen = nullptr;
}

void CraftingTile::interact(TileInteractionEvent t) {
	GUIElement* screen = GUI::$(uuid());
	if (screen != nullptr) {
		closeScreen();
		return;
	}
	screen = Components::basicCraftingMenu(input, output, crafter);
	GUI::setID(screen, uuid());
	GUI::root.appendChild(screen);
}

void CraftingTile::clientSideUpdate(bool canInteract) {
	GUIElement* screen = GUI::$(uuid());
	if (!canInteract && screen != nullptr) closeScreen();
}

void CraftingTile::update() {
	crafter->update();
}