#pragma once
#include "../../physics/physics.h"
#include "../../systems/inventory.h"

class Player {
private:
	const char* id;
public:
	Body<float>* body;
	Inventory inventory;

	Player(const char* playerID);
	~Player();
	Vector<float>& position();
};