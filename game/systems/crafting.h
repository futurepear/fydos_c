#pragma once
#include "dictionary.h"
#include "../constants.h"
#include "inventory.h"

struct CraftCommand {
	int recipeID;
	int ticksLeft;
	bool done = false;
	bool success = false;
};

CraftCommand createCraftCommand(int recipeID);

//assumes input storage has crafting inputs
void updateCraftCommand(CraftCommand& command, ItemBuffer* inputDest, ItemBuffer* outputDest);