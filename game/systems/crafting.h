#pragma once
#include "dictionary.h"
#include "../constants.h"
#include "inventory.h"
#include <queue>
#include <list>

//TODO: crafting authority

struct CraftCommand {
	int recipeID;
	int ticksLeft;
	bool done = false;
	bool success = true;
	ItemBuffer* inputDest = nullptr;
	ItemBuffer* outputDest = nullptr;
};

CraftCommand createCraftCommand(int recipeID);

//assumes input storage has crafting inputs
void updateCraftCommand(CraftCommand& command, ItemBuffer* inputDest, ItemBuffer* outputDest);

class Crafter {
private:
	std::list<CraftCommand> craftingQueue;
	ItemBuffer* inputDest = nullptr;
	ItemBuffer* outputDest = nullptr;
	bool automatic = false; //inventory automatic, crafting station not automatic, furnace automatic
	float craftingSpeed = 1.0;
	bool active = true; 
	int crafterID;
	int activeRecipe = 0;
public:
	Crafter(ItemBuffer* input, ItemBuffer* output, int crafterType = 0);
	void update();
	bool makesRecipe(int recipeID);
	void add(int recipeID);
	float progress();
	std::vector<int> findRecipes(std::vector<const ItemBuffer*> inputs);
};

