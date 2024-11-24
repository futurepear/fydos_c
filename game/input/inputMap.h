#pragma once

struct InputMap {
	bool W = false;
	bool A = false;
	bool S = false;
	bool D = false;
	bool R = false;
	bool M = false;

	bool mouseDown = false;
	int mouseX = 0;
	int mouseY = 0;

	int hotbar = 0;
};