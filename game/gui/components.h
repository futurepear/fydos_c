#pragma once
#include "core.h"
#include "../systems/crafting.h"
#include "../systems/inventory.h"

namespace Components {
	GUIElement* hotbar();
	GUIElement* inventory(ItemBuffer* inventoryObject);
	GUIElement* basicCraftingMenu(ItemBuffer* input, ItemBuffer* output, Crafter* crafter);
	GUIElement* scrollingContainer();
	const ItemBuffer* getContainerStorage(GUIElement* elem);

	class RecipeBrowser : public GUIElement {
	private:
		Crafter* crafter;
		GUIElement* recipeList;
	public:
		RecipeBrowser(Crafter* ref);
		void updateRecipeList(std::vector<const ItemBuffer*> recipeStorageInputs);
	};
}