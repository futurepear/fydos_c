#pragma once
#include "../../physics/physics.h"
#include "../../systems/inventory.h"
#include "../../input/inputMap.h"
#include "weapon.h"

class Player {
private:
	const char* id;
public:
	Body<float>* body;
	Inventory inventory;
	Usable<>* item;
	InputMap input{};

	Player(const char* playerID);
	~Player();
	Vector<float>& position();
	void update();
	float facing();
	void addInput(InputMap i);
	const Vector<float> mouse();
	void switchHotbarSlot(int slot);
	void setItem(ItemType type);
	void initializeHand();
};