#pragma once
#include "../../physics/physics.h"
#include "../../systems/inventory.h"
#include "../../input/inputMap.h"
#include "../../systems/crafting.h"
#include "weapon.h"
#include <queue>

class Player {
private:
	const char* id;
public:
	Body<float>* body;
	Inventory inventory;
	Usable<>* item{ nullptr };
	InputMap input{};

	Inventory craftingBuffer;
	std::queue<CraftCommand> craftingQueue;

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
	
	void updateCrafting();
	void craft(int recipeID);
};