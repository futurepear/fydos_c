#include "crafting.h"
#include "dictionary.h"
#include "../constants.h"
#include "inventory.h"
#include <queue>

CraftCommand createCraftCommand(int recipeID) {
	const Recipe& recipe = Dictionary::recipes[recipeID];
	int ticks = int(recipe.time * Constants::tps);
	CraftCommand command{ recipeID, ticks };
	return command;
}

void updateCraftCommand(CraftCommand& command, ItemBuffer* inputDest, ItemBuffer* outputDest) {
	command.ticksLeft--;
	if (command.ticksLeft <= 0) command.done = true;

	const Recipe& recipe = Dictionary::recipes[command.recipeID];

	//check if inputs are still there.. else theres a problem
	bool hasItems = true;
	for (auto& item : recipe.inputs) 
		hasItems = hasItems && inputDest->has(item.id, item.amount);

	bool inputOutputSame = (inputDest == outputDest);

	if (!outputDest->canFitRecipeOutputs(recipe, inputOutputSame)) command.done = false;

	if (!hasItems) {
		command.done = true;
		command.success = false;
	}

	if (command.done && command.success) {
		for (auto& item : recipe.inputs)
			inputDest->removeItem(item.id, item.amount);
		for (auto& item : recipe.outputs)
			outputDest->addItem(item.id, item.amount);
	}
}

Crafter::Crafter(ItemBuffer* input, ItemBuffer* output, int crafterType) : inputDest{ input }, outputDest{ output }, crafterID{ crafterType } {
	const auto& crafterData = Dictionary::crafters[crafterID];
	craftingSpeed = crafterData.craftingSpeed;
	automatic = crafterData.automatic;
}
void Crafter::update() {
	if (craftingQueue.empty()) return;
	CraftCommand& cmd = craftingQueue.front();

	updateCraftCommand(cmd, inputDest, outputDest);
	if (cmd.done) {
		if (cmd.success) { std::cout << "success"; } else { std::cout << "cancelled"; }
		craftingQueue.pop_front();
	}
}
bool Crafter::makesRecipe(int recipeID) {
	const auto& crafterData = Dictionary::crafters[crafterID];
	for (int i = 0; i < crafterData.recipes.size(); i++) {
		if (crafterData.recipes[i] == recipeID) return true;
	}
	return false;
}

void Crafter::add(int recipeID) {
	if (!makesRecipe(recipeID)) return;

	craftingQueue.push_back(createCraftCommand(recipeID));
}
float Crafter::progress() {
	if(craftingQueue.empty()) return 0.0f;
	CraftCommand& cmd = craftingQueue.front();
	const Recipe& recipe = Dictionary::recipes[cmd.recipeID];
	float ticks = recipe.time * Constants::tps;
	return 1.0f - (cmd.ticksLeft / ticks);
}

//assumes inputs includes its own input storage. (subtracts what is currently being used to craft from input storage)
std::vector<int> Crafter::findRecipes(std::vector<const ItemBuffer*> inputs) {

	ItemPool pool{}; //maps itemID to quantity

	inputDest->addToPool(pool);

	//remove items in queue from pool - assumes items in queue are in the pool
	for (auto const& craftCommand : craftingQueue) {
		const Recipe& recipe = Dictionary::recipes[craftCommand.recipeID];
		for (auto& item : recipe.inputs) {
			pool[item.id] -= item.amount;
		}
	}

	/*for (std::size_t i = 0; i < inputs.size(); i++) 
		inputs[i]->addToPool(pool);*/

	const auto& crafterData = Dictionary::crafters[crafterID];
	std::vector<int> recipesList{};

	for (int i = 0; i < crafterData.recipes.size(); i++) {	
		const auto& recipe = Dictionary::recipes[crafterData.recipes[i]];

		bool canCraft = true;
		for (int j = 0; j < recipe.inputs.size(); j++) {
			auto& input = recipe.inputs[j];
			canCraft = canCraft && (pool[input.id] >= input.amount);
		}
		if (canCraft)
			recipesList.push_back(recipe.id);
	}

	return recipesList;
}