#include "crafting.h"
#include "dictionary.h"
#include "../constants.h"
#include "inventory.h"

CraftCommand createCraftCommand(int recipeID) {
	const Recipe& recipe = Dictionary::recipes[recipeID];
	int ticks = recipe.time * Constants::tps;
	CraftCommand command{ recipeID, ticks };
	return command;
}

void updateCraftCommand(CraftCommand& command, ItemBuffer* inputDest, ItemBuffer* outputDest) {
	command.ticksLeft--;
	if (command.ticksLeft <= 0) command.done = true;

	if (command.done) {
		const Recipe& recipe = Dictionary::recipes[command.recipeID];
		//todo: check if room in output

		for (auto item : recipe.inputs)
			inputDest->removeItem(item.id, item.amount);
		for (auto item : recipe.outputs)
			outputDest->addItem(item.id, item.amount);

		command.success = true;
	}
}