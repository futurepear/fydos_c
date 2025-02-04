#pragma once
#include <string>
#include <vector>

class ItemBuffer;
class Crafter;

struct InputBuffer {
	std::string chat{};
	double scrollX = 0;
	double scrollY = 0;
};

struct InputMap {
	bool W = false;
	bool A = false;
	bool S = false;
	bool D = false;
	bool R = false;
	bool M = false;
	bool E = false;
	bool F = false;
	bool mouseDownGUI = false;
	bool mouseDown = false;
	bool rightMouseDown = false;
	int mouseX = 0;
	int mouseY = 0;
	int rawMouseX = 0;
	int rawMouseY = 0;

	int hotbarSlot = 0; 
	
	struct Cursor {
		ItemBuffer* storage = nullptr;
		int position = -1;
		int itemID = -1;
	};
	struct CraftInput {
		Crafter* target = nullptr;
		int recipeID = 0;
	};

	bool storageInteraction = false;
	Cursor cursor{};

	std::vector<CraftInput> crafts{};
};