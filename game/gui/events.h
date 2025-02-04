#pragma once
#include "../input/inputMap.h"

class Inventory;
class ItemBuffer;

struct MouseEvent {
	Inventory& inventory;
	InputMap& input;
	bool click = false;
	bool rclick = false;
	bool mousedown = false;
	bool mouseRelease = false;
	bool hold = false;
	bool shiftDown = false;
	bool ctrlDown = false;
	int x = 0;
	int y = 0;
	int dX = 0;
	int dY = 0;
	int scrollX = 0;
	int scrollY = 0;
};